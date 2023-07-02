cmake_minimum_required(VERSION 3.21)

project(Fractal LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Werror -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory("third_party")

add_executable(fractal)

target_sources(fractal
PUBLIC  
	src/main.cpp
)
            
target_compile_options(fractal PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(fractal 
PUBLIC
	src/shader
	src/camera
)
target_link_libraries(fractal OpenGL::GL glfw glad)
