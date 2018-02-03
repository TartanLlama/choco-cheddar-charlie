cmake_minimum_required(VERSION 2.6)

project(ChocoCheddarCharlie)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(ChocoCheddarCharlie
  src/main.cpp
  src/event_handler.cpp
  src/sprite_loader.cpp
  src/sprite.cpp)
target_include_directories(ChocoCheddarCharlie PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} include ext)
target_link_libraries(ChocoCheddarCharlie ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} stdc++fs)

# TODO proper modern CMake stuff
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -g -O0")
