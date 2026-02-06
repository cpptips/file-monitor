#!/bin/bash

echo "🎯 File Monitor 项目 GitHub 上传助手"
echo "======================================"

# 检查是否已连接到GitHub远程仓库
if git remote get-url origin > /dev/null 2>&1; then
    echo "✅ 已配置远程仓库: $(git remote get-url origin)"
else
    echo "❌ 尚未配置远程仓库"
    echo "请在GitHub上创建仓库后，将以下URL替换为您的实际仓库URL:"
    echo "git remote add origin https://github.com/cpptips/file-monitor.git"
    exit 1
fi

# 检查当前分支
CURRENT_BRANCH=$(git branch --show-current)
echo "📋 当前分支: $CURRENT_BRANCH"

# 推送代码
echo "🚀 开始推送代码到GitHub..."
if git push -u origin main; then
    echo "✅ 代码推送成功！"
    echo ""
    echo "🌐 您的项目现在可以在以下地址访问："
    echo "https://github.com/cpptips/file-monitor"
    echo ""
    echo "🎉 项目信息："
    echo "- 文件监控应用 (C++17)"
    echo "- 跨平台支持 (macOS/Linux/Windows)"
    echo "- 智能CPU调度"
    echo "- 大文件检测和报告"
else
    echo "❌ 推送失败，请检查："
    echo "1. GitHub仓库是否存在"
    echo "2. 网络连接"
    echo "3. 认证信息"
fi