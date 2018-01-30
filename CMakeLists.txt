cmake_minimum_required(VERSION 2.6)

project(ChocoCheddarCharlie)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)

add_executable(ChocoCheddarCharlie src/main.cpp)
target_include_directories(ChocoCheddarCharlie PRIVATE ${SDL2_INCLUDE_DIR} include)
target_link_libraries(ChocoCheddarCharlie ${SDL2_LIBRARY})

