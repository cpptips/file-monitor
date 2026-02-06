#!/bin/bash

# 文件监控程序启动脚本（带配置文件版本）
cd "$(dirname "$0")/file_monitor"

echo "启动文件监控程序（带配置文件）..."
./file_monitor_config config.json