#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include "tray.hpp"
#include "resource.h"

#define TRAY_ICON_ID 8358

void addTrayIcon(HINSTANCE hInstance, HWND hWnd) {
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hWnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = TRAY_NOTIFY;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    _tcscpy_s(nid.szTip, TEXT("ModernStandbyBluetoothOff"));
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void deleteTrayIcon(HWND hWnd) {
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hWnd;
    nid.uID = TRAY_ICON_ID;
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

bool processTrayMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (wParam == TRAY_ICON_ID && lParam == WM_RBUTTONUP) {
        auto hMenu = CreateMenu();
        auto hMenubar = CreateMenu();
        AppendMenu(hMenu, MF_STRING, MENU_QUIT_MESSAGE, TEXT("Quit"));
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR) hMenu, TEXT("Menu"));

        auto hPopupMenu = GetSubMenu(hMenubar, 0);
        POINT pt;
        GetCursorPos(&pt);
        SetForegroundWindow(hWnd);
        TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
        SetForegroundWindow(hWnd);
        PostMessage(hWnd, WM_NULL, 0, 0);

        DestroyMenu(hMenu);
        DestroyMenu(hMenubar);

        return true;
    }
    return false;
}
