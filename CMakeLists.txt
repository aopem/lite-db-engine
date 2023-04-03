cmake_minimum_required(VERSION 3.25)
project("db-engine-lite")

# use C++23 standard
set(CMAKE_CXX_STANDARD 23)

# for debugging
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# find Boost library
find_package(Boost 1.81 REQUIRED COMPONENTS
    log
)

# get all cpp files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "include/*.hpp" "include/*.cpp")

add_executable(main "src/main.cpp" ${SOURCES})
target_link_libraries(main PUBLIC
    Boost::boost
    Boost::log
)
target_include_directories(main PRIVATE "include/")
