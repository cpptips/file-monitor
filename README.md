# file_monitor - 智能文件监控工具 v1.0.0

![Version](https://img.shields.io/badge/Version-1.0.0-blue)
![Language](https://img.shields.io/badge/Language-C++17-green)
![Platform](https://img.shields.io/badge/Platform-Linux-orange)

一个高性能的C++文件监控工具，能够在系统CPU空闲时自动扫描用户目录，智能识别大文件并监控存储使用情况。

## 🚀 核心特性

### 智能监控
- **智能CPU触发**: 只有当CPU使用率低于阈值时才执行扫描，不影响系统性能
- **实时监控**: 持续监控指定的目录结构
- **资源友好**: 自动避让高负载时段，确保系统稳定性

### 丰富功能
- **📊 文件大小统计**: 自动识别并排序大文件
- **📁 目录大小监测**: 显示每个监控目录的总大小和文件数量
- **🔕 排除模式**: 支持排除指定目录和文件类型
- **🔄 定期扫描**: 可配置的扫描间隔和触发条件
- **📋 详细报告**: 生成易读的文本格式报告

### 双版本支持
- **基础版本**: 直接运行，使用内置默认配置
- **配置版本**: 支持完整的JSON配置文件，适合生产环境

## 📦 快速开始

### 安装发布包

```bash
# 下载并解压发布包
tar -xzf file_monitor_v1.0.0_linux_amd64.tar.gz
cd file_monitor_release_v1.0.0

# 安装到系统路径
sudo cp bin/file_monitor* /usr/local/bin/
sudo cp bin/start_monitor* /usr/local/bin/
```

### 直接运行

#### 基础版本（快速体验）
```bash
file_monitor
```

#### 配置版本（推荐生产使用）
```bash
file_monitor_config config/config.json
```

## ⚙️ 配置说明

### 配置文件示例 (`config.json`)

```json
{
    "directories": [
        "/home/users/niuguangxue",
        "/tmp"
    ],
    "exclude_patterns": [
        "*.tmp",
        "*.backup",
        ".git/*"
    ],
    "file_size_threshold_mb": 10,
    "cpu_usage_threshold_percent": 30,
    "scan_interval_seconds": 60,
    "output_file": "/home/users/niuguangxue/large_files_report.txt",
    "report_dir_sizes": true
}
```

### 配置项详解

| 配置项 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| `directories` | array | 必需 | 要监控的目录列表 |
| `exclude_patterns` | array | 可选 | 排除的文件模式 |
| `file_size_threshold_mb` | int | 10 | 文件大小阈值(MB) |
| `cpu_usage_threshold_percent` | double | 30.0 | CPU使用率阈值(%) |
| `scan_interval_seconds` | int | 60 | 扫描间隔(秒) |
| `output_file` | string | - | 输出文件路径 |
| `report_dir_sizes` | bool | true | 是否报告目录大小 |

## 📋 输出示例

程序会生成详细的报告文件：

```
文件监控报告
生成时间: Wed Feb  4 22:15:32 2026

监控配置:
- 监控目录数量: 4
- CPU触发阈值: 30%
- 扫描间隔: 60秒

目录大小统计:
- /home/users/niuguangxue: 9175 MB (169374 个文件)

占用存储最高的文件:
=================================
大小: 4834 MB	路径: "/home/users/niuguangxue/code/output/data/file"  
大小: 2083 MB	路径: "/home/users/niuguangxue/.cache/large_file.dat"
...
```

## 🛠️ 编译安装

### 从源码编译

```bash
# 克隆仓库
git clone <repository-url>
cd file_monitor_src

# 编译基础版本
/opt/compiler/gcc-12/bin/g++ -std=c++17 -Wall -Wextra -O2 -I. main.cpp -o file_monitor -lstdc++fs

# 编译配置版本
/opt/compiler/gcc-12/bin/g++ -std=c++17 -Wall -Wextra -O2 -I. main_with_config.cpp json.hpp -o file_monitor_config -lstdc++fs
```

### 使用Makefile（如果可用）
```bash
make all        # 编译所有版本
make clean      # 清理构建文件
```

## 🔧 技术架构

### 依赖项
- **C++17标准**: 使用现代C++特性
- **std::filesystem**: 文件系统操作
- **nlohmann/json**: JSON配置文件解析
- **Linux /proc文件系统**: CPU使用率监控

### 核心组件
- `main.cpp`: 基础版本主程序
- `main_with_config.cpp`: 配置版本主程序
- `json.hpp`: JSON解析库头文件
- 启动脚本: `start_monitor.sh`, `start_monitor_with_config.sh`

## 📈 性能优势

1. **高效扫描**: 使用递归目录遍历算法
2. **智能调度**: 仅在CPU空闲时运行
3. **内存友好**: 按需加载文件信息
4. **快速排序**: 高效的文件大小排序算法

## 🔍 使用场景

### 个人使用
- 监控个人目录存储使用情况
- 识别和清理大型临时文件
- 定期检查存储增长趋势

### 系统管理
- 服务器存储空间监控
- 用户目录配额管理
- 自动化存储维护任务

## 🐛 故障排除

### 常见问题

**Q: 程序无法访问某些目录**
A: 检查目录权限，确保程序有读取权限

**Q: CPU监控不准确**
A: 确保运行在Linux系统，/proc/stat文件可用

**Q: 报告文件未生成**
A: 检查输出文件路径权限和磁盘空间

### 调试模式
```bash
# 启用详细日志
file_monitor_config config.json 2>&1 | tee debug.log
```

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

## 🤝 贡献

欢迎提交 Issue 和 Pull Request 来改进这个项目！

## 📞 支持

如有问题请查看：
- [CHANGELOG.md](CHANGELOG.md) - 版本更新日志
- [RELEASE.md](RELEASE.md) - 发布说明

---

**file_monitor v1.0.0** - 智能、高效的文件监控解决方案