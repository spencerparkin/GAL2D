# CMakeLists.txt for GAL2D project.

cmake_minimum_required(VERSION 3.5)

project(GAL2D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GAL2D_BACKEND_OPENGL "Use OpenGL graphics backend for GAL2D." ON)
option(GAL2D_BACKEND_DX11 "Use DirectX 11 graphics backend for GAL2D." OFF)

add_subdirectory(Library)
add_subdirectory(Test)
add_subdirectory(Backend)