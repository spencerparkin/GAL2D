# CMakeLists.txt for GAL2D project.

cmake_minimum_required(VERSION 3.5)

project(GAL2D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED GAL2D_BACKEND)
	set(GAL2D_BACKEND "OpenGL")
endif()

function(LinkWithGAL2DBackend GivenTarget)
	if(GAL2D_BACKEND STREQUAL "OpenGL")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_OpenGL)
		target_compile_definitions(${GivenTarget} PUBLIC USING_OPENGL)
	elseif(GAL2D_BACKEND STREQUAL "DX11")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_DX11)
		target_compile_definitions(${GivenTarget} PUBLIC USING_DX11)
	elseif(GAL2D_BACKEND STREQUAL "DX12")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_DX12)
		target_compile_definitions(${GivenTarget} PUBLIC USING_DX12)
	elseif(GAL2D_BACKEND STREQUAL "DX12")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_METAL)
		target_compile_definitions(${GivenTarget} PUBLIC USING_METAL)
	elseif(GAL2D_BACKEND STREQUAL "VULKAN")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_VULKAN)
		target_compile_definitions(${GivenTarget} PUBLIC USING_VULKAN)
	else()
		message(FATAL_ERROR "Not sure which backend to link with in test program.")
	endif()
endfunction()

add_subdirectory(Library)
add_subdirectory(Test)
add_subdirectory(Backend)