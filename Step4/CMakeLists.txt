cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# for larger projects this is a common occurrence. The first step is to add an option to the top-level CMakeLists.txt file
option(USE_MY_MATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# conditionally execute a group of commands
if (USE_MY_MATH)

    # add the MathFunctions library
    add_subdirectory(MathFunctions)

    # The list subcommands APPEND, INSERT, FILTER, PREPEND, POP_BACK, POP_FRONT, REMOVE_AT, REMOVE_ITEM,
    #   REMOVE_DUPLICATES, REVERSE and SORT may create new values for the list within the current CMake variable scope.
    #   Similar to the set() command, the LIST command creates new variable values in the current scope, even if the
    #   list itself is actually defined in a parent scope.
    list(APPEND EXTRA_LIBS MathFunctions)

    # if we use target_include_directories on MathFunctions don't need this
    # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif ()

# add the MathFunctions library
# add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)

# EXTRA_LIBS come from 'list(APPEND EXTRA_LIBS MathFunctions)'
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# do not use conditionally
# target_include_libraries(Tutorial PUBLIC MathFunctions)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        # "${PROJECT_SOURCE_DIR}/MathFunctions"
        # id.
        # ${EXTRA_INCLUDES}
        )

# add the install targets
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
        DESTINATION include
        )

enable_testing()

# does the application run
add_test(NAME Runs COMMAND Tutorial 25)

# does the usage message work?
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
        PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
        )

# define a function to simplify adding tests
function(do_test target arg result)
    add_test(NAME Comp${arg} COMMAND ${target} ${arg})
    set_tests_properties(Comp${arg}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result}
            )
endfunction()

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is (-nan|nan|0)")
do_test(Tutorial 0.0001 "0.0001 is 0.01")