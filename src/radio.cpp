// Copyright (c) 2023 Park Hyunwoo
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <map>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Radios.h>
#include <winrt/Windows.System.h>

#include "debugLog.hpp"

using namespace winrt;
using namespace winrt::Windows::Devices::Radios;
using namespace winrt::Windows::Foundation;

std::map<hstring, RadioState> oldBluetoothStateMap;

void turnOffRadio()
{
    std::vector<IAsyncOperation<RadioAccessStatus>> ops;
    auto radioList = Radio::GetRadiosAsync().get();
    for (Radio radio : radioList)
    {
        if (radio.Kind() == RadioKind::Bluetooth)
        {
            auto it = oldBluetoothStateMap.find(radio.Name());
            if (it == oldBluetoothStateMap.end())
            {
                auto oldState = radio.State();
                oldBluetoothStateMap[radio.Name()] = oldState;
                //                ops.push_back(radio.SetStateAsync(RadioState::Off));
                debugLog(" - {}: {} -> Off(2)", to_string(radio.Name()), static_cast<int>(oldState));
            }
        }
    }
    // wait all SetStateAsync to complete
    for (auto &&op : ops)
    {
        op.get();
    }
}

void resumeRadio()
{
    std::vector<IAsyncOperation<RadioAccessStatus>> ops;
    for (Radio radio : Radio::GetRadiosAsync().get())
    {
        if (radio.Kind() == RadioKind::Bluetooth)
        {
            auto it = oldBluetoothStateMap.find(radio.Name());
            if (it != oldBluetoothStateMap.end()) {
                auto newState = it->second;
                ops.push_back(radio.SetStateAsync(newState));
                oldBluetoothStateMap.erase(it);
                debugLog(" - {}: Off(2) -> {}", to_string(radio.Name()), static_cast<int>(newState));
            }
        }
    }

    oldBluetoothStateMap.clear();

    // wait all SetStateAsync to complete
    for (auto &&op: ops) {
        op.get();
    }
}
