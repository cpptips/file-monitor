#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>  // 改为标准C++17文件系统
#include <algorithm>
#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>
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
using json = nlohmann::json;

// 跨平台CPU使用率获取
double get_cpu_usage() {
#if OS_WINDOWS
    static ULARGE_INTEGER lastIdleTime, lastKernelTime, lastUserTime;
    FILETIME idleTime, kernelTime, userTime;
    
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;
    
    ULONGLONG idleDiff = idle.QuadPart - lastIdleTime.QuadPart;
    ULONGLONG kernelDiff = kernel.QuadPart - lastKernelTime.QuadPart;
    ULONGLONG userDiff = user.QuadPart - lastUserTime.QuadPart;
    
    lastIdleTime = idle;
    lastKernelTime = kernel;
    lastUserTime = user;
    
    ULONGLONG total = kernelDiff + userDiff;
    return (total > 0) ? (100.0 - (100.0 * idleDiff) / total) : 0.0;
#elif OS_MACOS
    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, 
                       (host_info_t)&cpuinfo, &count) == KERN_SUCCESS) {
        unsigned long total = cpuinfo.cpu_ticks[CPU_STATE_USER] +
                             cpuinfo.cpu_ticks[CPU_STATE_SYSTEM] +
                             cpuinfo.cpu_ticks[CPU_STATE_NICE] +
                             cpuinfo.cpu_ticks[CPU_STATE_IDLE];
        if (total > 0) {
            unsigned long used = total - cpuinfo.cpu_ticks[CPU_STATE_IDLE];
            return (double)used * 100.0 / total;
        }
    }
    return 0.0;
#else // Linux
    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);
    
    unsigned long long user, nice, system, idle, iowait, irq, softirq;
    std::istringstream iss(line.substr(5));
    iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq;
    
    static unsigned long long prev_total_idle = idle + iowait;
    static unsigned long long prev_total_non_idle = user + nice + system + irq + softirq;
    
    unsigned long long total_idle = idle + iowait;
    unsigned long long total_non_idle = user + nice + system + irq + softirq;
    
    unsigned long long total_idle_diff = total_idle - prev_total_idle;
    unsigned long long total_non_idle_diff = total_non_idle - prev_total_non_idle;
    unsigned long long total_diff = total_idle_diff + total_non_idle_diff;
    
    prev_total_idle = total_idle;
    prev_total_non_idle = total_non_idle;
    
    return (total_diff > 0) ? (double)total_non_idle_diff / total_diff * 100.0 : 0.0;
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

// 其余原有代码保持不变...