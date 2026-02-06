# File Monitor

一个跨平台的文件系统监控应用，用于检测大文件并生成报告。

## 功能特性

- 🔍 **智能文件扫描**: 递归扫描目录并分析文件大小
- ⚡ **CPU感知**: 仅在CPU使用率低时执行扫描
- 🌐 **跨平台支持**: 支持macOS、Linux、Windows
- 📊 **报告生成**: 生成占用存储最高的文件列表
- ⚙️ **可配置**: 支持JSON配置文件

## 编译安装

### 系统要求
- C++17兼容编译器 (gcc 8+, clang 7+, MSVC 2019+)
- Make

### 使用Make编译
```bash
make
```

## 使用方法

### 基本使用
```bash
./file_monitor
```

程序将：
1. 扫描用户主目录
2. 检测CPU使用率，仅在低于30%时扫描
3. 每60秒检查一次
4. 生成报告到 `~/large_files_report.txt`

## 项目结构

```
file-monitor/
├── main_fixed.cpp      # 主应用程序（跨平台CPU监控）
├── main_with_config.cpp # 配置版本
├── json.hpp            # JSON解析库
├── Makefile            # 构建配置
├── config.json         # 示例配置文件
├── start_monitor.sh    # 启动脚本
└── README.md          # 项目说明
```

## 编译状态

✅ **已测试平台**: macOS (arm64)

## 贡献指南

1. Fork 项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建Pull Request

## 许可证

项目采用MIT许可证。

## 联系方式

- 项目主页: https://github.com/cpptips/file-monitor
- 问题反馈: [GitHub Issues](https://github.com/cpptips/file-monitor/issues)