#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <thread>
// 平台相关头文件
#if defined(_WIN32)
    #include <windows.h>
    #include <psapi.h>
#elif defined(__APPLE__)
    #include <mach/mach.h>
#else
    #include <sys/statvfs.h>
    #include <sys/sysinfo.h>
#endif
#include <ctime>
#include <sstream>

namespace fs = std::filesystem;

struct FileInfo {
    fs::path path;
    uintmax_t size;
};

// 跨平台CPU使用率获取
double get_cpu_usage() {
#if defined(_WIN32)
    static ULARGE_INTEGER lastIdleTime = {}, lastKernelTime = {}, lastUserTime = {};
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
#elif defined(__APPLE__)
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
#else
    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);
    
    unsigned long long user, nice, system, idle, iowait, irq, softirq;
    std::istringstream iss(line.substr(5));
    iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq;
    
    unsigned long long total_idle = idle + iowait;
    unsigned long long total_non_idle = user + nice + system + irq + softirq;
    
    static unsigned long long prev_total_idle = 0;
    static unsigned long long prev_total_non_idle = 0;
    
    if (prev_total_idle == 0 || prev_total_non_idle == 0) {
        prev_total_idle = total_idle;
        prev_total_non_idle = total_non_idle;
        return 0.0;
    }
    
    unsigned long long total_idle_diff = total_idle - prev_total_idle;
    unsigned long long total_non_idle_diff = total_non_idle - prev_total_non_idle;
    unsigned long long total_diff = total_idle_diff + total_non_idle_diff;
    
    prev_total_idle = total_idle;
    prev_total_non_idle = total_non_idle;
    
    return (total_diff > 0) ? (double)total_non_idle_diff / total_diff * 100.0 : 0.0;
#endif
}

// 扫描目录并收集文件信息
void scan_directory(const fs::path& dir_path, std::vector<FileInfo>& files) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
            if (fs::is_regular_file(entry.status())) {
                try {
                    uintmax_t file_size = fs::file_size(entry.path());
                    files.push_back({entry.path(), file_size});
                } catch (const fs::filesystem_error&) {
                    continue; // 跳过无法访问的文件
                }
            }
        }
    } catch (const fs::filesystem_error&) {
        std::cerr << "无法访问目录: " << dir_path << std::endl;
    }
}

// 比较文件大小
bool compare_file_size(const FileInfo& a, const FileInfo& b) {
    return a.size > b.size;
}

// 主监控循环
void monitor_loop(const fs::path& scan_path, const fs::path& output_path) {
    const double LOW_CPU_THRESHOLD = 30.0; // CPU使用率低于30%时执行扫描
    const int SCAN_INTERVAL = 60; // 扫描间隔(秒)
    
    while (true) {
        double cpu_usage = get_cpu_usage();
        std::cout << "当前CPU使用率: " << cpu_usage << "%" << std::endl;
        
        if (cpu_usage < LOW_CPU_THRESHOLD) {
            std::cout << "CPU使用率低，开始扫描..." << std::endl;
            
            std::vector<FileInfo> files;
            scan_directory(scan_path, files);
            
            // 按文件大小排序
            std::sort(files.begin(), files.end(), compare_file_size);
            
            // 输出结果到文件
            std::ofstream out_file(output_path);
            if (out_file.is_open()) {
                auto now = std::chrono::system_clock::now();
                std::time_t now_time = std::chrono::system_clock::to_time_t(now);
                out_file << "扫描时间: " << std::ctime(&now_time);
                out_file << "占用存储最高的文件:\n";
                out_file << "=================================\n";
                
                const size_t max_files_to_show = 100;
                for (size_t i = 0; i < std::min(files.size(), max_files_to_show); ++i) {
                    out_file << "大小: " << files[i].size / (1024 * 1024) << " MB\t路径: " 
                             << files[i].path << "\n";
                }
                
                std::cout << "扫描完成，结果已保存到: " << output_path << std::endl;
            } else {
                std::cerr << "无法打开输出文件: " << output_path << std::endl;
            }
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(SCAN_INTERVAL));
    }
}

// 跨平台主目录获取
fs::path get_home_directory() {
#if defined(_WIN32)
    return fs::path(std::getenv("USERPROFILE"));
#else
    const char* home = std::getenv("HOME");
    return home ? fs::path(home) : fs::current_path();
#endif
}

int main() {
    fs::path home_dir = get_home_directory();
    fs::path output_file = home_dir / "large_files_report.txt";
    
    std::cout << "文件监控程序已启动\n";
    std::cout << "将扫描用户目录: " << home_dir << "\n";
    std::cout << "结果将输出到: " << output_file << "\n";
    std::cout << "程序将在CPU使用率低时自动扫描...\n";
    
    monitor_loop(home_dir, output_file);
    
    return 0;
}