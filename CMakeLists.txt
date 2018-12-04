cmake_minimum_required(VERSION 3.5)

project(particles-demo)

#so the compile_commands.json is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include directories
#include_directories(include /home/julian/Programs/glfw-build/include)
include_directories(include/ include_other/)

#wildcard source additions
#file(GLOB SOURCES "src/*.cpp")
file(GLOB_RECURSE SOURCES "src/*.cpp")
#file(GLOB SOURCES "stb-src/*.cpp")

#manually setting sources
#set(SOURCES "stb-src/stb_image.cpp")

#compiler/linker flags
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(GCC_COVERAGE_LINK_FLAGS "-lglfw3 -lGL -lX11 -lXi -lXrandr -ldl -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(particles-demo ${SOURCES})

#GLFW specific
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-build)

target_link_libraries(particles-demo glfw)

find_package(OpenGL REQUIRED)
include_directories(particles-demo ${OPENGL_INCLUDE_DIR})
target_link_libraries(particles-demo ${OPENGL_gl_LIBRARY})
