# CMakeLists.txt for OpenGL graphics implementation of GAL2D on Windows.

set(GAL2D_WINGL_SOURCES
	Source/GraphicsWinGL.cpp
	Source/GraphicsWinGL.h
	Source/TextureWinGL.cpp
	Source/TextureWinGL.h
	Source/GAL2DMakeInterface.cpp
	Source/GAL2DMakeInterface.h
)

add_library(GAL2D_WinGL STATIC ${GAL2D_WINGL_SOURCES})

target_include_directories(GAL2D_WinGL PUBLIC Source)

target_link_libraries(GAL2D_WinGL PUBLIC
	GAL2D_MSWindows
	opengl32.lib
	glu32.lib
)