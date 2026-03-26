# CMakeLists.txt for GAL2D project.

cmake_minimum_required(VERSION 3.5)

project(GAL2D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED GAL2D_BACKEND)
	set(GAL2D_BACKEND "WinGL")
endif()

function(LinkWithGAL2DBackend GivenTarget)
	if(GAL2D_BACKEND STREQUAL "WinGL")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_WinGL)
		target_compile_definitions(${GivenTarget} PUBLIC USING_WIN_GL)
	elseif(GAL2D_BACKEND STREQUAL "WinDX11")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_WinDX11)
		target_compile_definitions(${GivenTarget} PUBLIC USING_WIN_DX11)
	elseif(GAL2D_BACKEND STREQUAL "WinDX12")
		target_link_libraries(${GivenTarget} PUBLIC GAL2D_WinDX12)
		target_compile_definitions(${GivenTarget} PUBLIC USING_WIN_DX12)
	else()
		message(FATAL_ERROR "Not sure which backend to link with in test program.")
	endif()
endfunction()

add_subdirectory(Library)
add_subdirectory(Test)
add_subdirectory(Backend)