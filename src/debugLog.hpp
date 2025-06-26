// Copyright (c) 2023 Park Hyunwoo
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <windows.h>

#define debugLog(fmt, ...) logA(std::format(fmt, ##__VA_ARGS__))
void logA(const std::string& message);
