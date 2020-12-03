cmake_minimum_required (VERSION 3.8)

project("VisibleSurfaceRendering")

set(EXECUTABLE_NAME "VisibleSurfaceRendering")

set(FILE_SOURCES
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml/imgui-SFML.cpp" # Workaround for ImGui-SFML
	"Source/Main.cpp"
	"Source/ExampleShape.cpp"
	"Source/Core/App.cpp"
	"Source/Misc/BinaryTreePartitioning.cpp"
	"Source/Component/TransformComponent.cpp"
	"Source/Component/Shape3DComponent.cpp"
	"Source/Component/SurfaceComponent.cpp"
	"Source/Component/Surface2DComponent.cpp"
	"Source/System/BaseRenderSystem.cpp"
	"Source/System/PainterRenderSystem.cpp"
	"Source/System/ZBufferRenderSystem.cpp"
	"Source/System/TransformSystem.cpp"
	"Source/Util/Color.cpp"
	"Source/Util/ModelLoader/Lexer.cpp"
	"Source/Misc/EdgeBucket.cpp"
	 "Source/Util/ModelLoader/Parser.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(${EXECUTABLE_NAME}
		${FILE_SOURCES})
	target_compile_definitions(${EXECUTABLE_NAME} PUBLIC _DEBUG_BUILD)
else()
	add_executable(${EXECUTABLE_NAME}
		WIN32
		${FILE_SOURCES})
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${EXECUTABLE_NAME} PUBLIC "-std=c++17")
	# Compile with static runtime
	target_compile_options(${EXECUTABLE_NAME} PUBLIC "-static-libgcc -static-libstdc++")
	# Resize the stack size
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS -Wl,--stack,8000000)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(${EXECUTABLE_NAME} PUBLIC "/std:c++17")
	# Compile with static runtime
	target_compile_options(${EXECUTABLE_NAME} PUBLIC "/MTd")
	# Resize the stack size
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS /STACK:8000000)
endif()

if(WIN32)
	# If x64
	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
		if(MSVC)
			target_compile_definitions(${EXECUTABLE_NAME} PUBLIC
				SFML_STATIC)
			if(CMAKE_BUILD_TYPE STREQUAL "Debug")
				target_link_libraries(${EXECUTABLE_NAME} PUBLIC
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-window-s-d-srt.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-graphics-s-d-srt.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-system-s-d-srt.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/freetype.lib")
			else()
				target_link_libraries(${EXECUTABLE_NAME} PUBLIC
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-window-s.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-graphics-s.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-system-s.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/freetype.lib")
			endif()
		elseif(MINGW)
			target_compile_definitions(${EXECUTABLE_NAME}
					SFML_STATIC)
			if(CMAKE_BUILD_TYPE STREQUAL "Debug")
				target_link_libraries(${EXECUTABLE_NAME} PUBLIC
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-window-s-d.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-graphics-s-d.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-system-s-d.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libfreetype.a")
			else()
				target_link_libraries(${EXECUTABLE_NAME} PUBLIC
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-window-s.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-graphics-s.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-system-s.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libfreetype.a")
			endif()
		endif()
	else()
		message(FATAL_ERROR "x64-bit compiler required")
	endif()
else()
	message(FATAL_ERROR "This program is intended for Windows")
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
target_compile_options("ImGui" PUBLIC "/MTd")

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
	# See Lib/ogdf/lib/README.md if you have any difficulties
	"${CMAKE_SOURCE_DIR}/Lib/ogdf/lib/OGDF-d-srt.lib"
	"${CMAKE_SOURCE_DIR}/Lib/ogdf/lib/COIN-d-srt.lib"
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
