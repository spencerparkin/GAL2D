# CMakeLists.txt for GAL2D test program.

set(GAL2D_TEST_SOURCES
	Source/Main.cpp
	Source/Main.h
	Source/App.cpp
	Source/App.h
)

add_executable(GAL2D_Test WIN32 ${GAL2D_TEST_SOURCES})

if(GAL2D_BACKEND_OPENGL)
	target_link_libraries(GAL2D_Test PUBLIC GAL2D_OpenGL)
	target_compile_definitions(GAL2D_Test PUBLIC USING_OPENGL)
elseif(GAL2D_BACKEND_DX11)
	target_link_libraries(GAL2D_Test PUBLIC GAL2D_DX11)
	target_compile_definitions(GAL2D_Test PUBLIC USING_DX11)
elseif(GAL2D_BACKEND_DX12)
	target_link_libraries(GAL2D_Test PUBLIC GAL2D_DX12)
	target_compile_definitions(GAL2D_Test PUBLIC USING_DX12)
elseif(GAL2D_BACKEND_METAL)
	target_link_libraries(GAL2D_Test PUBLIC GAL2D_METAL)
	target_compile_definitions(GAL2D_Test PUBLIC USING_METAL)
elseif(GAL2D_BACKEND_VOLKAN)
	target_link_libraries(GAL2D_test PUBLIC GAL2D_VULKAN)
	target_compile_definitions(GAL2D_Test PUBLIC USING_VULKAN)
else()
	message(FATAL_ERROR "Not sure which backend to link with in test program.")
endif()