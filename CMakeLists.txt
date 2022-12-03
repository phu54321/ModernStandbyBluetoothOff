cmake_minimum_required(VERSION 3.0.0)
project(ModernStandbyBluetoothOff VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

add_executable(ModernStandbyBluetoothOff WIN32
  tray.cpp
  radio.cpp
  main.cpp
  debugLog.cpp
  resource.rc
  )
