cmake_minimum_required(VERSION 3.21)

project(Sierpinski LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Werror -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory("third_party")

add_executable(sierpinski)

target_sources(sierpinski
PUBLIC  
	includes/shader.hpp
	includes/camera.hpp
	includes/pyramid.hpp
	includes/renderer.hpp

	src/main.cpp
)
            
target_compile_options(sierpinski PRIVATE -Wall -Wextra -Wpedantic -O3)

target_include_directories(sierpinski 
PUBLIC
	includes/
)
target_link_libraries(sierpinski OpenGL::GL glfw glad)
