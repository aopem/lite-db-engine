cmake_minimum_required(VERSION 3.25)
project("db-engine-lite")

# use C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# for debugging
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# option to build tests
option(BUILD_TESTS "Build test executables" OFF)

# find Boost library using CONFIG mode (modern approach)
find_package(Boost 1.89 REQUIRED COMPONENTS log CONFIG)

# get all cpp files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "include/*.hpp" "include/*.cpp")

# create a shared library with all the common code
add_library(litedb_core STATIC ${SOURCES})
target_link_libraries(litedb_core PUBLIC
    Boost::boost
    Boost::log
)
target_include_directories(litedb_core PUBLIC "include/")

# main executable
add_executable(main "src/main.cpp")
target_link_libraries(main PRIVATE litedb_core)

# build tests if enabled
if(BUILD_TESTS)
    enable_testing()

    # find all test files
    file(GLOB_RECURSE TEST_SOURCES "src/test/**/*.cpp")

    # create an executable for each test file
    foreach(TEST_SOURCE ${TEST_SOURCES})
        # get filename without path and extension
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        # create executable
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE litedb_core)

        # add test to CTest
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()

    message(STATUS "Test executables will be built")
endif()
