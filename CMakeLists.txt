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

set(GRAPHVIZ_LIB
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/gvc.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/cgraph.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/cdt.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/Pathplan.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/xdot.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/gvplugin_core.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/gvplugin_gdiplus.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/gvplugin_neato_layout.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/gvplugin_dot_layout.lib"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/lib/gvplugin_pango.lib")

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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS -Wl,--stack,6000000)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	if (MSVC_VERSION GREATER_EQUAL "1900")
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			target_compile_options(${EXECUTABLE_NAME} PUBLIC "/std:c++latest")
		endif()
	endif()
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS /STACK:6000000)
endif()

if(WIN32)
	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
		if(MSVC)
			target_compile_definitions(${EXECUTABLE_NAME} PUBLIC
				SFML_STATIC)
			if(CMAKE_BUILD_TYPE STREQUAL "Debug")
				target_link_libraries(${EXECUTABLE_NAME} PUBLIC
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-window-s-d.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-graphics-s-d.lib"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-system-s-d.lib"
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


target_link_libraries(${EXECUTABLE_NAME} PUBLIC
	${GRAPHVIZ_LIB}
	"ImGui"
	"opengl32.lib"
	"winmm.lib"
	"gdi32.lib")

target_include_directories(${EXECUTABLE_NAME} PUBLIC
	"${CMAKE_SOURCE_DIR}/Lib/SFML/include"
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml"
	"${CMAKE_SOURCE_DIR}/Lib/graphviz/include"
	"${CMAKE_SOURCE_DIR}/Lib/imgui"
	"${CMAKE_SOURCE_DIR}/Lib/entt/single_include"
	"${CMAKE_SOURCE_DIR}/Lib/glm")

