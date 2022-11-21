#pragma once

#include <windows.h>

#define MENU_QUIT_MESSAGE 0x101
#define TRAY_NOTIFY (WM_APP + 100)

void addTrayIcon(HINSTANCE hInstance, HWND hWnd);
bool processTrayMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
