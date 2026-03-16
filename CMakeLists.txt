# CMakeLists.txt for GAL2D project.

cmake_minimum_required(VERSION 3.5)

project(GAL2D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GAL2D_BACKEND "OpenGL")

add_subdirectory(Library)
add_subdirectory(Test)
add_subdirectory(Backend)