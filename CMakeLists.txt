cmake_minimum_required(VERSION 3.16)

project(unnamed2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(unnamed2
    src/main.cpp
    src/state.cpp
    src/camera.cpp
    src/shader.cpp)

if(MSVC)
  target_compile_options(unnamed2 PRIVATE /W4)
else()
  target_compile_options(unnamed2 PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(unnamed2 PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(unnamed2 PRIVATE GLEW::GLEW)
target_link_libraries(unnamed2 PRIVATE glm::glm)

