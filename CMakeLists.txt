cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(PROJECT_NAME "node-fontinfo")
project(${PROJECT_NAME})

set(freetype2_INSTALL ${CMAKE_BINARY_DIR}/freetype2)

include(ucm)
include(ExternalProject)

ExternalProject_Add(
	freetype2
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/freetype2
	URL "https://download.savannah.gnu.org/releases/freetype/freetype-2.9.tar.gz"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${freetype2_INSTALL}
)

set(fontinfo_SOURCES
	src/fontinfo/fontinfo.c)
	
set(fontinfo_HEADERS
	src/fontinfo/fontinfo.h
	src/fontinfo/endian.h)

add_library(fontinfo STATIC ${fontinfo_SOURCES})

target_include_directories(fontinfo PUBLIC
	"${freetype2_INSTALL}/include/freetype2")

target_link_libraries(fontinfo
	${CMAKE_JS_LIB}
	${freetype2_INSTALL}/lib/freetype${CMAKE_STATIC_LIBRARY_SUFFIX})

add_dependencies(fontinfo freetype2)


add_library(${PROJECT_NAME} SHARED "src/module.cpp")
target_link_libraries(${PROJECT_NAME} fontinfo ${CMAKE_JS_LIB})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_JS_INC})

if(FONTINFO_BUILD_TESTS)
	add_executable(simple_test "test/simple_test.c")
	target_link_libraries(simple_test fontinfo)
	target_include_directories(simple_test PUBLIC "src")
endif()