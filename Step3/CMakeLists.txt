cmake_minimum_required(VERSION 3.10)

project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_MY_MATH "Use tutorial provided math implementation" ON)

configure_file(TutorialConfig.h.in TutorialConfig.h)

if (USE_MY_MATH)

    add_subdirectory(MathFunctions)

    list(APPEND EXTRA_LIBS MathFunctions)

    # 如果我们直接在MathFunctions的CMakeLists.txt下进行配置target_include_directories可省略这一步
    # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif ()

add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        )