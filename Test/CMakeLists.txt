# CMakeLists.txt for GAL2D test program.

set(GAL2D_TEST_SOURCES
	Source/Main.cpp
	Source/Main.h
)

add_executable(GAL2D_Test WIN32 ${GAL2D_TEST_SOURCES})

LinkWithGAL2DBackend(GAL2D_Test)