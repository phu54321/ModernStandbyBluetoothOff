#include <windows.h>
#include <powrprof.h>
#include <tlhelp32.h>
#include <string>
#include <algorithm>
#include <set>
#include <tchar.h>
#include <iostream>
#include <future>

#include "winrt/Windows.Foundation.h"

#include "resource.h"
#include "tray.hpp"
#include "radio.hpp"
#include "debugLog.hpp"

using namespace winrt;

HPOWERNOTIFY g_hPowerNotify;

bool ProcessSuspendResumeNotification(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_POWERBROADCAST) {
        switch (wParam) {
            case PBT_APMSUSPEND:
                logA("PBT_APMSUSPEND");
                logA("Turning bluetooth OF...");
                turnOffRadio();
                return true;

                // case PBT_APMRESUMEAUTOMATIC:
            case PBT_APMRESUMESUSPEND:
                logA("PBT_APMRESUMESUSPEND");
                logA("Turning bluetooth ON...");
                resumeRadio();
                return true;
        }
    }
    return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ProcessSuspendResumeNotification(hWnd, msg, wParam, lParam))
        return TRUE;

    if (msg == TRAY_NOTIFY) {
        if (processTrayMessage(hWnd, msg, wParam, lParam))
            return 0;
    } else if (msg == WM_COMMAND) {
        auto wmId = LOWORD(wParam);
        auto wmEvent = HIWORD(wParam);

        if (wmId == MENU_QUIT_MESSAGE) {
            PostQuitMessage(0);
            return 0;
        }
    } else if (msg == WM_DESTROY) {
        if (g_hPowerNotify) {
            UnregisterSuspendResumeNotification(g_hPowerNotify);
            g_hPowerNotify = nullptr;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

///

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    winrt::init_apartment();

    // Disable double execution
    auto mutex = CreateMutex(nullptr, TRUE, TEXT("Global\\ModernStandbyBluetoothOff"));
    if (!mutex || GetLastError()) {
        MessageBox(nullptr, TEXT("Another instance already running."), TEXT("Error"), MB_OK);
        return 0;
    }

    WNDCLASS wndClass;
    memset(&wndClass, 0, sizeof(WNDCLASS));
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = TEXT("ModernStandbyBluetoothOff");
    wndClass.lpfnWndProc = WndProc;
    RegisterClass(&wndClass);

    HWND hWnd = CreateWindowEx(
            WS_EX_LEFT,
            TEXT("ModernStandbyBluetoothOff"),
            TEXT("ModernStandbyBluetoothOff"),
            0,
            0,
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            hInstance,
            nullptr);

    if (!hWnd) {
        MessageBox(nullptr, TEXT("Cannot create window"), TEXT("Error"), MB_OK);
        return 1;
    }

    addTrayIcon(hInstance, hWnd);
    g_hPowerNotify = RegisterSuspendResumeNotification(hWnd, DEVICE_NOTIFY_WINDOW_HANDLE);

    logA("ModernStandbyBluetoothOff Starting...");
    MSG msg = {};

    while (GetMessage(&msg, hWnd, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    logA("ModernStandbyBluetoothOff stopped");
    return 0;
}
