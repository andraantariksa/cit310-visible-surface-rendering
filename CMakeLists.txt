cmake_minimum_required (VERSION 3.8)

project ("Spheros")

set(EXECUTABLE_NAME "Spheros")
add_executable(${EXECUTABLE_NAME}
	"Source/Main.cpp")

if (WIN32)
	target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
		SFML_STATIC)
	target_include_directories(${EXECUTABLE_NAME} PRIVATE
		"${CMAKE_SOURCE_DIR}/Lib/SFML-Windows-x64/include")
	target_link_libraries(${EXECUTABLE_NAME} PRIVATE
		"${CMAKE_SOURCE_DIR}/Lib/SFML-Windows-x64/lib/sfml-window-s-d.lib"
		"${CMAKE_SOURCE_DIR}/Lib/SFML-Windows-x64/lib/sfml-graphics-s-d.lib"
		"${CMAKE_SOURCE_DIR}/Lib/SFML-Windows-x64/lib/sfml-system-s-d.lib"
		"${CMAKE_SOURCE_DIR}/Lib/SFML-Windows-x64/lib/freetype.lib"
		"opengl32.lib"
		"winmm.lib"
		"gdi32.lib")
endif()
