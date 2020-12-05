cmake_minimum_required (VERSION 3.8)

project("VisibleSurfaceRendering")

set(EXECUTABLE_NAME "VisibleSurfaceRendering")

# Compiler vibe check
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# Compile with static runtime
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# Compile with static runtime
	add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

set(BUILD_SHARED_LIBS FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)
add_subdirectory("${CMAKE_SOURCE_DIR}/Lib/SFML")

configure_file(
	"${CMAKE_SOURCE_DIR}/Lib/ogdf_SvgPrinter.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/ogdf/src/ogdf/fileformats/SvgPrinter.cpp"
	COPYONLY)
add_subdirectory("${CMAKE_SOURCE_DIR}/Lib/ogdf")
configure_file(
	"${CMAKE_CURRENT_BINARY_DIR}/Lib/ogdf/include/ogdf/basic/internal/config_autogen.h"
	"${CMAKE_SOURCE_DIR}/Lib/ogdf/include/ogdf/basic/internal/config_autogen.h"
	COPYONLY)

set(FILE_SOURCES
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml/imgui-SFML.cpp" # Workaround for ImGui-SFML
	"Source/Main.cpp"
	"Source/ExampleShape.cpp"
	"Source/Core/App.cpp"
	"Source/Misc/BinaryTreePartitioning.cpp"
	"Source/Component/TransformComponent.cpp"
	"Source/Component/Shape3DComponent.cpp"
	"Source/Component/Surface3DComponent.cpp"
	"Source/Component/Triangle2DComponent.cpp"
	"Source/System/BaseRenderSystem.cpp"
	"Source/System/PainterRenderSystem.cpp"
	"Source/System/ZBufferRenderSystem.cpp"
	"Source/System/TransformSystem.cpp"
	"Source/Util/Color.cpp"
	"Source/Util/ModelLoader/Lexer.cpp"
	"Source/Misc/EdgeBucket.cpp"
	 "Source/Util/ModelLoader/Parser.cpp"
	 "Source/Component/Triangle3DComponent.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(${EXECUTABLE_NAME}
		${FILE_SOURCES})
	target_compile_definitions(${EXECUTABLE_NAME} PUBLIC _DEBUG_BUILD)
else()
	add_executable(${EXECUTABLE_NAME}
		WIN32
		${FILE_SOURCES})
endif()

# Compiler vibe check
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${EXECUTABLE_NAME} PUBLIC "-std=c++17")
	# Resize the stack size
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS -Wl,--stack,8000000)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(${EXECUTABLE_NAME} PUBLIC "/std:c++17")
	# Resize the stack size
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS /STACK:8000000)
endif()

# ImGui
add_library("ImGui" STATIC
	"${CMAKE_SOURCE_DIR}/Lib/imgui/imgui.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/imgui/imgui_draw.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/imgui/imgui_widgets.cpp"
	#"${CMAKE_SOURCE_DIR}/Lib/imgui/misc/cpp/imgui_stdlib.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/imgui/misc/fonts/binary_to_compressed_c.cpp"
	#"${CMAKE_SOURCE_DIR}/Lib/imgui/misc/freetype/imgui_freetype.cpp"
	)
target_compile_options("ImGui" PUBLIC)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
	"sfml-graphics"
    "sfml-system"
    "sfml-window"
	"OGDF"
	"COIN"
	"ImGui"
	"opengl32.lib"
	"winmm.lib"
	"gdi32.lib")

target_include_directories(${EXECUTABLE_NAME} PUBLIC
	"${CMAKE_SOURCE_DIR}/Lib/ogdf/include"
	"${CMAKE_SOURCE_DIR}/Lib/SFML/include"
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml"
	"${CMAKE_SOURCE_DIR}/Lib/imgui"
	"${CMAKE_SOURCE_DIR}/Lib/entt/single_include"
	"${CMAKE_SOURCE_DIR}/Lib/portable-file-dialogs"
	"${CMAKE_SOURCE_DIR}/Lib/glm")
