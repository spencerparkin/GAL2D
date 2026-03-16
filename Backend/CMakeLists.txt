# CMakeLists.txt for various GAL2D backends.

if(GAL2D_BACKEND STREQUAL "OpenGL")
	add_subdirectory(OpenGL)
elseif(GAL2D_BACKEND STREQUAL "DX11")
	add_subdirectory(DirectX11)
else()
	message(FATAL_ERROR "No graphics backend specified!")
endif()