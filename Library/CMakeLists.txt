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
	Source/Graphics/Font.cpp
	Source/Graphics/Font.h
	Source/ThirdParty/stb_image.cpp
	Source/ThirdParty/stb_image.h
	Source/ThirdParty/stb_image_write.cpp
	Source/ThirdParty/stb_image_write.h
)

add_library(GAL2D STATIC ${GAL2D_SOURCES})

target_include_directories(GAL2D PUBLIC Source)

target_include_directories(GAL2D PRIVATE
	ThirdParty/freetype/include
)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    target_link_directories(GAL2D PUBLIC
        ThirdParty/freetype/objs/x64/DebugStatic
    )
else()
    target_link_directories(GAL2D PUBLIC
        ThirdParty/freetype/objs/x64/ReleaseStatic
    )
endif()

target_link_libraries(GAL2D PRIVATE
	freetype.lib
)

target_compile_definitions(GAL2D PRIVATE
	_USE_MATH_DEFINES
	_CRT_SECURE_NO_WARNINGS
)