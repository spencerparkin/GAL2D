# CMakeLists.txt for OpenGL graphics implementation of GAL2D.

set(GAL2D_OPENGL_SOURCES
	Source/GraphicsOpenGL.cpp
	Source/GraphicsOpenGL.h
	Source/TextureOpenGL.cpp
	Source/TextureOpenGL.h
	Source/GAL2DMakeInterface.cpp
	Source/GAL2DMakeInterface.h
)

add_library(GAL2D_OpenGL STATIC ${GAL2D_OPENGL_SOURCES})

target_include_directories(GAL2D_OpenGL PUBLIC Source)

target_link_libraries(GAL2D_OpenGL PUBLIC
	GAL2D
	opengl32.lib
	glu32.lib
)