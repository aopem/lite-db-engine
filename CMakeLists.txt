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

# get all source files (excluding tests and main)
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

# create a static library with all the common code
add_library(litedb_core STATIC ${LIB_SOURCES})
target_link_libraries(litedb_core PUBLIC
    Boost::boost
    Boost::log
)

# public headers (API for library users)
target_include_directories(litedb_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# private headers (internal implementation)
target_include_directories(litedb_core PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# interactive terminal executable
add_executable(litedb_terminal "tools/litedb_terminal.cpp")
target_link_libraries(litedb_terminal PRIVATE litedb_core)
target_include_directories(litedb_terminal PRIVATE ${CMAKE_SOURCE_DIR}/src)

# build tests if enabled
if(BUILD_TESTS)
    enable_testing()

    # find all test files
    file(GLOB_RECURSE TEST_SOURCES "test/**/*.cpp")

    # create an executable for each test file
    foreach(TEST_SOURCE ${TEST_SOURCES})
        # get filename without path and extension
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        # create executable
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE litedb_core)
        target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

        # add test to CTest
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()

    message(STATUS "Test executables will be built")
endif()
