cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial1 VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure a header file to pass some of the CMake settings
# to the source code
# https://cmake.org/cmake/help/latest/command/configure_file.html
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the executable
add_executable(Tutorial1 tutorial.cxx)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
# https://cmake.org/cmake/help/latest/command/target_include_directories.html?highlight=target_include_directories
# https://cmake.org/cmake/help/latest/variable/PROJECT_BINARY_DIR.html?highlight=project_binary_dir
target_include_directories(Tutorial1 PUBLIC
		"${PROJECT_BINARY_DIR}"
		)
