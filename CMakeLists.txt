cmake_minimum_required(VERSION 3.15)

project(ray-tracing)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -O3")

include_directories(${CURSES_INCLUDE_DIRS})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(${PROJECT_NAME} main.cpp ${sources} ${headers})