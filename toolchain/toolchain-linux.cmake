# 目标系统名称
# set(CMAKE_SYSTEM_NAME Linux)

# 目标架构
# set(CMAKE_SYSTEM_PROCESSOR x86_64)  # 64 位 Linux
# set(CMAKE_SYSTEM_PROCESSOR i686)  # 32 位 Linux

# 指定交叉编译器
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# 设置目标平台
# set(CMAKE_C_COMPILER_TARGET x86_64-linux-gnu)  # 64 位 Linux
# set(CMAKE_C_COMPILER_TARGET i686-linux-gnu)  # 32 位 Linux

# 查找库和头文件的路径
# set(CMAKE_FIND_ROOT_PATH /usr/x86_64-linux-gnu)

# 从来不在指定目录(交叉编译)下查找工具程序。(编译时利用的是宿主的工具)
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# # 只在指定目录(交叉编译)下查找库文件
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# # 只在指定目录(交叉编译)下查找头文件
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# # 只在指定的目录(交叉编译)下查找依赖包
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)