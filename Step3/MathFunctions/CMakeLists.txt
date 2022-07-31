add_library(MathFunctions mysqrt.cpp)

# 任何连接这个库的人都需要包含当前源文件夹，才能找到MathFunctions.h
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
