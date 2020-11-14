cmake_minimum_required (VERSION 3.8)

project ("VisibleSurfaceRendering")

set(EXECUTABLE_NAME "VisibleSurfaceRendering")

set(FILE_SOURCES
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml/imgui-SFML.cpp" # Workaround for ImGui-SFML
	"Source/Main.cpp"
	"Source/ExampleShape.cpp"
	"Source/Core/App.cpp"
	"Source/Core/Camera.cpp"
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
	"Source/Misc/EdgeBucket.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(${EXECUTABLE_NAME}
		${FILE_SOURCES})
	target_compile_definitions(${EXECUTABLE_NAME} PRIVATE _DEBUG_BUILD)
else()
	add_executable(${EXECUTABLE_NAME}
		WIN32
		${FILE_SOURCES})
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS -Wl,--stack,5000000)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	if (MSVC_VERSION GREATER_EQUAL "1900")
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			target_compile_options(${EXECUTABLE_NAME} PRIVATE "/std:c++latest")
		endif()
	endif()
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS /STACK:5000000)
endif()

if(WIN32)
	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
		if(MSVC)
			target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
				SFML_STATIC)
			if(CMAKE_BUILD_TYPE STREQUAL "Debug")
				target_link_libraries(${EXECUTABLE_NAME} PRIVATE
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-window-s-d.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-graphics-s-d.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-system-s-d.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/freetype.lib"
					"opengl32.lib"
					"winmm.lib"
					"gdi32.lib")
			else()
				target_link_libraries(${EXECUTABLE_NAME} PRIVATE
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-window-s.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-graphics-s.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-system-s.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/freetype.lib"
					"opengl32.lib"
					"winmm.lib"
					"gdi32.lib")
			endif()
		elseif(MINGW)
			target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
					SFML_STATIC)
			if(CMAKE_BUILD_TYPE STREQUAL "Debug")
				target_link_libraries(${EXECUTABLE_NAME} PRIVATE
						"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-window-s-d.a"
						"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-graphics-s-d.a"
						"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-system-s-d.a"
						"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libfreetype.a"
						"opengl32.lib"
						"winmm.lib"
						"gdi32.lib")
			else()
				target_link_libraries(${EXECUTABLE_NAME} PRIVATE
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-window-s.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-graphics-s.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-system-s.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libfreetype.a"
					"opengl32.lib"
					"winmm.lib"
					"gdi32.lib")
			endif()
		endif()
	endif()
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


target_link_libraries(${EXECUTABLE_NAME} PRIVATE
	"ImGui")

target_include_directories(${EXECUTABLE_NAME} PRIVATE
	"${CMAKE_SOURCE_DIR}/Lib/SFML/include"
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml"
	"${CMAKE_SOURCE_DIR}/Lib/imgui"
	"${CMAKE_SOURCE_DIR}/Lib/entt/single_include"
	"${CMAKE_SOURCE_DIR}/Lib/glm")