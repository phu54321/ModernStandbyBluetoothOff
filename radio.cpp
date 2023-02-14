#include <map>
#include "debugLog.hpp"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Radios.h>
#include <winrt/Windows.System.h>

using namespace winrt;
using namespace winrt::Windows::Devices::Radios;
using namespace winrt::Windows::Foundation;

std::map<hstring, RadioState> oldBluetoothStateMap;

IAsyncAction turnOffRadio() {
    std::vector<IAsyncOperation<RadioAccessStatus>> ops;
    for (Radio radio: co_await Radio::GetRadiosAsync()) {
        if (radio.Kind() == RadioKind::Bluetooth) {
            auto it = oldBluetoothStateMap.find(radio.Name());
            if (it != oldBluetoothStateMap.end()) {
                auto oldState = radio.State();
                oldBluetoothStateMap[radio.Name()] = oldState;
                ops.push_back(radio.SetStateAsync(RadioState::Off));
                logA(" - %s: %d -> Off(2)", to_string(radio.Name()).c_str(), oldState);
            }
        }
    }
    // wait all SetStateAsync to complete
    for (auto &&op: ops) {
        co_await op;
    }
}

IAsyncAction resumeRadio() {
    std::vector<IAsyncOperation<RadioAccessStatus>> ops;
    for (Radio radio: Radio::GetRadiosAsync().get()) {
        if (radio.Kind() == RadioKind::Bluetooth) {
            auto it = oldBluetoothStateMap.find(radio.Name());
            if (it != oldBluetoothStateMap.end()) {
                auto newState = it->second;
                ops.push_back(radio.SetStateAsync(newState));
                oldBluetoothStateMap.erase(it);
                logA(" - %s: Off(2) -> %d", to_string(radio.Name()).c_str(), newState);
            }
        }
    }
    // wait all SetStateAsync to complete
    for (auto &&op: ops) {
        co_await op;
    }
}
