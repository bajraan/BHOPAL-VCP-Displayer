// AppEnvironment.cpp
#include "AppEnvironment.hpp"

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif

std::string getExecutableDir()
{
    #if defined(_WIN32)
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return fs::path(buffer).parent_path().string();
    #elif defined(__linux__)
        char buffer[1024];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
        if (len != -1) {
            buffer[len] = '\0';
            return fs::path(buffer).parent_path().string();
        } else {
            throw std::runtime_error("Could not determine executable path on Linux");
        }
    #elif defined(__APPLE__)
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            return fs::path(buffer).parent_path().string();
        } else {
            throw std::runtime_error("Could not determine executable path on macOS");
        }
    #else
        throw std::runtime_error("Unsupported platform");
    #endif
}