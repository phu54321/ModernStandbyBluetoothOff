cmake_minimum_required(VERSION 3.20)
project(ModernStandbyBluetoothOff VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

add_executable(ModernStandbyBluetoothOff WIN32
        src/tray.cpp
        src/radio.cpp
        src/main.cpp
        res/resource.rc
        )
