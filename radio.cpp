#include <map>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Radios.h>
#include <winrt/Windows.System.h>

using namespace winrt;
using namespace winrt::Windows::Devices::Radios;
using namespace winrt::Windows::Foundation;

std::map<hstring, RadioState> oldBluetoothStateMap;

void turnOffRadio() {
    std::vector<IAsyncOperation < RadioAccessStatus>>
    ops;
    for (Radio radio: Radio::GetRadiosAsync().get()) {
        if (radio.Kind() == RadioKind::Bluetooth) {
            oldBluetoothStateMap[radio.Name()] = radio.State();
            ops.push_back(radio.SetStateAsync(RadioState::Off));
        }
    }
    // wait all SetStateAsync to complete
    for (auto &op: ops) {
        op.get();
    }
}

void resumeRadio() {
    std::vector<IAsyncOperation < RadioAccessStatus>>
    ops;
    for (Radio radio: Radio::GetRadiosAsync().get()) {
        if (radio.Kind() == RadioKind::Bluetooth) {
            auto it = oldBluetoothStateMap.find(radio.Name());
            if (it != oldBluetoothStateMap.end()) {
                ops.push_back(radio.SetStateAsync(it->second));
                oldBluetoothStateMap.erase(it);
            }
        }
    }
    // wait all SetStateAsync to complete
    for (auto &op: ops) {
        op.get();
    }
}
