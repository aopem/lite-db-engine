cmake_minimum_required(VERSION 3.25)
project("db-engine-lite")

# use C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# for debugging
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find Boost library using CONFIG mode (modern approach)
find_package(Boost 1.89 REQUIRED COMPONENTS log CONFIG)

# get all cpp files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "include/*.hpp" "include/*.cpp")

add_executable(main "src/main.cpp" ${SOURCES})
target_link_libraries(main PUBLIC
    Boost::boost
    Boost::log
)
target_include_directories(main PRIVATE "include/")
