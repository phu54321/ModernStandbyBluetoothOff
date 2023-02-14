#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt::Windows::Foundation;

IAsyncAction turnOffRadio();

IAsyncAction resumeRadio();
