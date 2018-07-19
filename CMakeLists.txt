cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(node_fontinfo)

set(freetype2_INSTALL ${CMAKE_BINARY_DIR}/freetype2)

include(NodeJS)
include(ExternalProject)

nodejs_init()

ExternalProject_Add(
	freetype2
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/freetype2
	URL "https://download-mirror.savannah.gnu.org/releases/freetype/freetype-2.9.tar.gz"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${freetype2_INSTALL}
)

set(freetype2_INCLUDE_DIRS ${freetype2_INSTALL}/include/freetype2)
set(freetype2_DEBUG_LIBRARY
	${freetype2_INSTALL}/lib/freetyped${CMAKE_STATIC_LIBRARY_SUFFIX})

set(freetype2_RELEASE_LIBRARY
	${freetype2_INSTALL}/lib/freetype${CMAKE_STATIC_LIBRARY_SUFFIX})

set(fontinfo_SOURCES
	src/fontinfo/fontinfo.c)

set(fontinfo_HEADERS
	src/fontinfo/fontinfo.h
	src/fontinfo/endian.h)

add_library(fontinfo STATIC ${fontinfo_SOURCES})

target_include_directories(fontinfo PUBLIC ${freetype2_INCLUDE_DIRS})

target_link_libraries(fontinfo
	optimized ${freetype2_RELEASE_LIBRARY}
	debug ${freetype2_DEBUG_LIBRARY})

add_dependencies(fontinfo freetype2)

add_nodejs_module(node_fontinfo "src/module.cpp")
target_link_libraries(node_fontinfo fontinfo ${NODEJS_LIBRARIES})
target_include_directories(node_fontinfo PUBLIC ${NODEJS_INCLUDE_DIRS})

if(FONTINFO_BUILD_TESTS)
	add_executable(simple_test "test/simple_test.c")
	target_link_libraries(simple_test fontinfo)
	target_include_directories(simple_test PUBLIC "src")
endif()

install(FILES $<TARGET_PDB_FILE:node_fontinfo> DESTINATION ${CMAKE_INSTALL_PREFIX} OPTIONAL)
install(FILES $<TARGET_FILE:node_fontinfo> DESTINATION ${CMAKE_INSTALL_PREFIX})

install(
	FILES
		${node_fontinfo_SOURCE_DIR}/package.json
		${node_fontinfo_SOURCE_DIR}/main.js
	DESTINATION
		${CMAKE_INSTALL_PREFIX})