// Copyright (c) 2023 Park Hyunwoo
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <tchar.h>

#include "../res/resource.h"
#include "tray.hpp"
#include "radio.hpp"
#include "debugLog.hpp"

using namespace winrt;

HPOWERNOTIFY g_hPowerNotify;
UINT WM_TASKBARCREATED = -1;

bool ProcessSuspendResumeNotification(HWND rhWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_POWERBROADCAST)
    {
        switch (wParam)
        {
        case PBT_APMSUSPEND:
            debugLog("[PBT_APMSUSPEND] Turning bluetooth OFF...");
            turnOffRadio();
            return true;

        case PBT_APMRESUMESUSPEND:
            debugLog("[PBT_APMRESUMESUSPEND] Turning bluetooth ON...");
            resumeRadio();
            return true;

        default:;
        }
    }
    return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ProcessSuspendResumeNotification(hWnd, msg, wParam, lParam))
        return TRUE;

    if (msg == WM_TASKBARCREATED) {
        auto hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
        addTrayIcon(hInstance, hWnd);
        return 0;
    }

    if (msg == TRAY_NOTIFY) {
        if (processTrayMessage(hWnd, msg, wParam, lParam)) {
            return 0;
        }
    } else if (msg == WM_COMMAND) {
        auto wmId = LOWORD(wParam);
        //        auto wmEvent = HIWORD(wParam);

        if (wmId == MENU_QUIT_MESSAGE)
        {
            PostQuitMessage(0);
            return 0;
        }
    }
    else if (msg == WM_DESTROY)
    {
        if (g_hPowerNotify)
        {
            UnregisterSuspendResumeNotification(g_hPowerNotify);
            g_hPowerNotify = nullptr;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

///

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    WM_TASKBARCREATED = RegisterWindowMessageW(L"TaskbarCreated");

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

    if (!hWnd)
    {
        MessageBox(nullptr, TEXT("Cannot create window"), TEXT("Error"), MB_OK);
        return 1;
    }

    addTrayIcon(hInstance, hWnd);
    g_hPowerNotify = RegisterSuspendResumeNotification(hWnd, DEVICE_NOTIFY_WINDOW_HANDLE);

    debugLog("ModernStandbyBluetoothOff Starting...");
    MSG msg = {};

    while (GetMessage(&msg, hWnd, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    debugLog("ModernStandbyBluetoothOff stopped");
    deleteTrayIcon(hWnd);
    return 0;
}
