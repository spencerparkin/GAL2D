# CMakeLists.txt for MS-windows-based graphics implementation of GAL2D.

set(GAL2D_WIN_SOURCES
	Source/GraphicsMSWindows.cpp
	Source/GraphicsMSWindows.h
)

add_library(GAL2D_MSWindows STATIC ${GAL2D_WIN_SOURCES})

target_include_directories(GAL2D_MSWindows PUBLIC Source)

target_link_libraries(GAL2D_MSWindows PUBLIC
	GAL2D
)