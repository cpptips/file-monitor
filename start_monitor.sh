#!/bin/bash

# 文件监控程序启动脚本
# 使用方法: ./start_monitor.sh 或 ./start_monitor.sh -d (后台运行)

cd "$(dirname "$0")"

echo "编译文件监控程序..."
/opt/compiler/gcc-12/bin/g++ -std=c++17 -Wall -Wextra -O2 file_monitor/main.cpp -o file_monitor -lstdc++fs

if [ $? -eq 0 ]; then
    echo "编译成功！"
    
    if [ "$1" = "-d" ]; then
        echo "以守护进程方式启动监控程序..."
        nohup ./file_monitor > monitor.log 2>&1 &
        echo "监控程序已在后台运行，日志输出到 monitor.log"
        echo "可以使用 'ps aux | grep file_monitor' 查看进程状态"
    else
        echo "启动监控程序..."
        ./file_monitor
    fi
else
    echo "编译失败，请检查错误信息"
    exit 1
fi