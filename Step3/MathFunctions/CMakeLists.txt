# add the following one line CMakeLists.txt file to the MathFunctions directory
add_library(MathFunctions mysqrt.cpp)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
