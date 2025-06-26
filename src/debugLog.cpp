#include <Windows.h>
#include <Shlobj.h>

#include <cstdio>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <filesystem>

static std::wstring getLogPath() {
    static bool initialized = false;
    static std::wstring path;

    if (!initialized) {
        WCHAR dir[MAX_PATH] = {0};
        SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, dir);
        path = std::wstring(dir) + L"\\ModernStandbyBluetoothOff.log";
    }
    return path;
}

void logA(const std::string& message) {
    OutputDebugStringA(("[ModernStandbyBluetoothOff] " + message).c_str());

    auto logPath = getLogPath();
    if (std::filesystem::exists(logPath)) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::fstream of(logPath, std::ios_base::app);
        of << std::put_time(&tm, "[%d-%m-%Y %H-%M-%S] ") << message << std::endl;
    }
}
