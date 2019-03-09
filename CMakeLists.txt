cmake_minimum_required(VERSION 3.1)
project(statictest)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

include_directories(src)
include_directories(src/listeners)
include_directories(src/sensors)
include_directories(lib)
include_directories(include)

file(GLOB SRC_FILES  src/*.cpp src/listeners/*.cpp src/sensors/*.cpp lib/*.cpp)
file(GLOB SQLITE_FILES lib/sqlite3.c)

add_definitions(-DTHREADS=10)

add_library(sqlite STATIC ${SQLITE_FILES})

add_executable(statictest ${SRC_FILES})

find_package(Threads REQUIRED)
target_link_libraries(statictest Threads::Threads )

target_link_libraries(statictest g3logger)

target_link_libraries(statictest sqlite)

target_link_libraries(statictest dl)