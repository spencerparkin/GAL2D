# CMakeLists.txt for GAL2D static library.

set(GAL2D_SOURCES
	Source/Math/Vector.cpp
	Source/Math/Vector.h
	Source/Math/Color.cpp
	Source/Math/Color.h
	Source/Math/Rectangle.cpp
	Source/Math/Rectangle.h
	Source/Math/LinearTransform.cpp
	Source/Math/LinearTransform.h
	Source/Math/AffineTransform.cpp
	Source/Math/AffineTransform.h
	Source/Graphics/GraphicsInterface.cpp
	Source/Graphics/GraphicsInterface.h
	Source/Graphics/Texture.cpp
	Source/Graphics/Texture.h
	Source/Graphics/Image.cpp
	Source/Graphics/Image.h
)

add_library(GAL2D STATIC ${GAL2D_SOURCES})

target_include_directories(GAL2D PUBLIC Source)

target_compile_definitions(GAL2D PRIVATE _USE_MATH_DEFINES)