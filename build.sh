#!/bin/bash

# RTI DDS Micro 构建脚本
# 用于编译DataType项目

set -e  # 遇到错误时退出

echo "开始构建RTI DDS Micro项目..."

# 创建构建目录
mkdir -p build
cd build

# 配置CMake
echo "配置CMake..."
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

# 编译项目
echo "编译项目..."
make -j$(nproc)

echo "构建完成！"
echo "可执行文件位置: $(pwd)/bin/test"

# 显示可执行文件信息
if [ -f "bin/test" ]; then
    echo "可执行文件大小: $(ls -lh bin/test | awk '{print $5}')"
    echo "运行示例:"
    echo "  发布者: ./bin/test pub -domain 0 -sleep 1000 -count 10"
    echo "  订阅者: ./bin/test sub -domain 0"
else
    echo "警告: 可执行文件未找到"
fi 