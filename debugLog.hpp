#pragma once

#include <windows.h>

#define debugLog(fmt, ...) OutputDebugStringA(std::format(fmt, ##__VA_ARGS__).c_str())
