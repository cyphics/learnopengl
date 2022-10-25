cmake_minimum_required(VERSION 3.23)
project(opengl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw-3.3.8)
include_directories(include/)

find_library(LIB glfw3 lib)

add_executable(opengl
        src/main.cpp
        src/glad.c
        src/shader.h)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw  ${GLFW_LIBRARY} ${OPENGL_LIBRARY})
#target_link_libraries(${ALL_LIBS})

