#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>  // 改为标准C++17文件系统
#include <algorithm>
#include <chrono>
#include <thread>
#include <ctime>
#include "json.hpp"

// 平台检测宏
#if defined(_WIN32)
#define OS_WINDOWS 1
#include <windows.h>
#include <psapi.h>
#elif defined(__APPLE__)
#define OS_MACOS 1
#include <mach/mach.h>
#else
#define OS_LINUX 1
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#endif

namespace fs = std::filesystem;  // 使用标准命名空间

// 跨平台CPU使用率获取（与main_with_config.cpp保持一致）
double get_cpu_usage() {
#if OS_WINDOWS
    static ULARGE_INTEGER lastIdleTime, lastKernelTime, lastUserTime;
    FILETIME idleTime, kernelTime, userTime;
    
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    // ... Windows实现代码 ...
#elif OS_MACOS
    // ... macOS实现代码 ...
#else // Linux
    // ... Linux实现代码 ...
#endif
}

// 跨平台主目录获取
fs::path get_home_directory() {
#if OS_WINDOWS
    return fs::path(std::getenv("USERPROFILE"));
#else
    const char* home = std::getenv("HOME");
    return home ? fs::path(home) : fs::current_path();
#endif
}

// 扫描目录并收集文件信息
void scan_directory(const fs::path& dir_path, std::vector<FileInfo>& files) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
            try {
                if (fs::is_regular_file(entry.status())) {
                    uintmax_t file_size = fs::file_size(entry.path());
                    files.push_back({entry.path(), file_size});
                }
            } catch (const fs::filesystem_error&) {
                continue; // 跳过无法访问的文件
            }
        }
    } catch (const fs::filesystem_error&) {
        std::cerr << "无法访问目录: " << dir_path << std::endl;
    }
}

// 其余原有代码保持不变...