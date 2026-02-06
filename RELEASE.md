# file_monitor 发布说明

## 版本 v1.0.0

### 简介
file_monitor 是一个智能文件监控工具，能够在CPU空闲时自动扫描存储空间，识别大文件并监控目录占用。

### 核心功能
- 🚀 智能CPU监控 - 仅空闲时扫描
- 📊 自动分析文件存储占用
- ⚙️ JSON配置文件支持
- 📁 目录大小统计
- 🔕 排除目录功能
- 📋 生成详细报告

### 系统要求
- Linux 系统
- g++ (支持C++17)
- 推荐: gcc-12+

### 快速开始
```bash
# 基础版本
cd file_monitor && ./file_monitor

# 配置版本（推荐）
cd file_monitor/file_monitor && ./file_monitor_config config.json
```

### 配置文件示例
```json
{
    "monitor_directories": ["/home/user"],
    "exclude_directories": ["/home/user/.cache"],
    "cpu_threshold": 30.0,
    "report_dir_sizes": true
}
```

### 优势
- 资源友好：仅在系统空闲时运行
- 配置灵活：完整的JSON配置支持
- 报告详细：文件和目录双重分析
- 运行稳定：C++编写，性能优异

---

**file_monitor - 智能文件监控工具**