cmake_minimum_required (VERSION 3.8)

project ("Spheros")

set(EXECUTABLE_NAME "Spheros")
add_executable(${EXECUTABLE_NAME}
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml/imgui-SFML.cpp" # Workaround for ImGui-SFML
	"Source/Main.cpp"
	"Source/Core/App.cpp"
	"Source/Component/TransformComponent.cpp"
	"Source/Component/SphereComponent.cpp"
	"Source/System/RenderSystem.cpp")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	if (MSVC_VERSION GREATER_EQUAL "1900")
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			target_compile_options(${EXECUTABLE_NAME} PRIVATE "/std:c++latest")
		endif()
	endif()
endif()

if(WIN32)
	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
		if(MSVC)
			target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
				SFML_STATIC)
			target_link_libraries(${EXECUTABLE_NAME} PRIVATE
				"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-window-s-d.lib"
				"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-graphics-s-d.lib"
				"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/sfml-system-s-d.lib"
				"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/msvc-win-x64/freetype.lib"
				"opengl32.lib"
				"winmm.lib"
				"gdi32.lib")
		elseif(MINGW)
			target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
					SFML_STATIC)
			target_link_libraries(${EXECUTABLE_NAME} PRIVATE
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-window-s-d.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-graphics-s-d.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libsfml-system-s-d.a"
					"${CMAKE_SOURCE_DIR}/Lib/SFML/lib/mingw-win-x64/libfreetype.a"
					"opengl32.lib"
					"winmm.lib"
					"gdi32.lib")
		endif()
	endif()
endif()

# ImGui
add_library("ImGui" STATIC
	"${CMAKE_SOURCE_DIR}/Lib/imgui/imgui.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/imgui/imgui_demo.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/imgui/imgui_draw.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/imgui/imgui_widgets.cpp"
	#"${CMAKE_SOURCE_DIR}/Lib/imgui/misc/cpp/imgui_stdlib.cpp"
	"${CMAKE_SOURCE_DIR}/Lib/imgui/misc/fonts/binary_to_compressed_c.cpp"
	#"${CMAKE_SOURCE_DIR}/Lib/imgui/misc/freetype/imgui_freetype.cpp"
	 "Source/Component/SphereComponent.hpp")


# ImGui-SFML
# Unresolved external symbol for no reason
#add_library("ImGui-SFML" STATIC
#	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml/imgui-SFML.cpp")
#if (WIN32)
#	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
#		target_link_libraries("ImGui-SFML" PRIVATE
#			"${CMAKE_SOURCE_DIR}/Lib/SFML/win-x64-lib/sfml-window-s-d.lib"
#			"${CMAKE_SOURCE_DIR}/Lib/SFML/win-x64-lib/sfml-graphics-s-d.lib"
#			"${CMAKE_SOURCE_DIR}/Lib/SFML/win-x64-lib/sfml-system-s-d.lib"
#			"${CMAKE_SOURCE_DIR}/Lib/SFML/win-x64-lib/freetype.lib"
#			"opengl32.lib"
#			"winmm.lib"
#			"gdi32.lib")
#	endif()
#endif()
#target_link_libraries("ImGui-SFML" PRIVATE
#	"ImGui")
#target_include_directories("ImGui-SFML" PRIVATE
#	"${CMAKE_SOURCE_DIR}/Lib/imgui"
#	"${CMAKE_SOURCE_DIR}/Lib/SFML/include")


target_link_libraries(${EXECUTABLE_NAME} PRIVATE
#	"ImGui-SFML"
	"ImGui")

target_include_directories(${EXECUTABLE_NAME} PRIVATE
	"${CMAKE_SOURCE_DIR}/Lib/SFML/include"
	"${CMAKE_SOURCE_DIR}/Lib/imgui-sfml"
	"${CMAKE_SOURCE_DIR}/Lib/imgui"
	"${CMAKE_SOURCE_DIR}/Lib/entt/single_include"
	"${CMAKE_SOURCE_DIR}/Lib/glm")