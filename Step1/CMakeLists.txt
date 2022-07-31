# cmake 的最低要求版本号
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(Tutorial VERSION 1.0)

# 指定C++标准版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 配置cmake头文件，用于设置一些头属性变量
# https://cmake.org/cmake/help/latest/command/configure_file.html
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 添加可执行文件
# https://cmake.org/cmake/help/latest/command/add_executable.html?highlight=add_executable
add_executable(Tutorial tutorial.cxx)

# 添加依赖目录，用于查询例如上面configure_file生成的头文件
# https://cmake.org/cmake/help/latest/command/target_include_directories.html?highlight=target_include_directories
# https://cmake.org/cmake/help/latest/variable/PROJECT_BINARY_DIR.html?highlight=project_binary_dir
target_include_directories(Tutorial PUBLIC
		"${PROJECT_BINARY_DIR}"
		)
