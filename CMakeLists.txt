cmake_minimum_required(VERSION 3.27)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(utilityKnife)

if(MSVC)
	if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
		add_definitions(/MP)
	endif()
endif()

# set up engine
set(ENGINE_NAME ${PROJECT_NAME})
set(ENGINE_SRC_PATH "engine/src")
set(ENGINE_INC_PATH "engine/include")
file(GLOB_RECURSE ENGINE_SOURCES
	"${ENGINE_SRC_PATH}/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS
	"${ENGINE_INC_PATH}/*.hpp")

add_library(${ENGINE_NAME} STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
source_group("Source Files"	FILES ${ENGINE_SOURCES})
source_group("Header Files" FILES ${ENGINE_HEADERS})
target_include_directories(${ENGINE_NAME} PUBLIC ${ENGINE_INC_PATH})
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)

# set up editor
set(EDITOR_NAME "utl_Editor")
set(EDITOR_SRC_PATH "editor/src")
set(EDITOR_INC_PATH "editor/include")
file(GLOB_RECURSE EDITOR_SOURCES
	"${EDITOR_SRC_PATH}/*.cpp")
file(GLOB_RECURSE EDITOR_HEADERS
	"${EDITOR_INC_PATH}/*.hpp")
	
add_executable(${EDITOR_NAME} ${EDITOR_SOURCES} ${EDITOR_HEADERS})
set_target_properties(${EDITOR_NAME} 
	PROPERTIES WIN32_EXECUTABLE $<AND:$<PLATFORM_ID:Windows>,$<CONFIG:Release>>)
source_group("Source Files" FILES ${EDITOR_SOURCES})
source_group("Header Files" FILES ${EDITOR_HEADERS})
target_include_directories(${EDITOR_NAME} PUBLIC ${EDITOR_INC_PATH})
target_compile_features(${EDITOR_NAME} PUBLIC cxx_std_20)


if(MSVC)
	target_compile_options(${ENGINE_NAME} PRIVATE
		/W4
		/permissive-
		/w44365
		/external:W3
	)

	target_compile_options(${EDITOR_NAME} PRIVATE
		/W4
		/permissive-
		/w44365
		/external:W3
	)
else()
	target_compile_options(${ENGINE_NAME} PRIVATE
		-pedantic
		-pedantic-errors
		-Wall
		-Wextra
		-Wconversion
		-Wsign-conversion
		-Werror
		-std=c++20
	)

	target_compile_options(${EDITOR_NAME} PRIVATE
		-pedantic
		-pedantic-errors
		-Wall
		-Wextra
		-Wconversion
		-Wsign-conversion
		-Werror
		-std=c++20
	)
endif()

# set up SDL

if ((APPLE AND NOT CMAKE_SYSTEM_NAME MATCHES "Darwin") OR EMSCRIPTEN)
	set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
	set(SDL_SHARED OFF)
	set(SDL_STATIC ON)
else()
	set(SDL_SHARED ON)
	set(SDL_STATIC OFF)
endif()

set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_DISABLE_INSTALL ON)
set(SDL_DISABLE_INSTALL_DOCS ON)
set(SDL_INSTALL_TESTS OFF)
add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)

set(SDLTTF_VENDORED ON)
add_subdirectory(extern/SDL_ttf EXCLUDE_FROM_ALL)

set(SDLMIXER_MIDI_NATIVE OFF)
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)
set(SDLMIXER_MOD OFF)
set(SDLMIXER_MOD_XMP OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_MP3_MPG123 OFF)
set(SDLMIXER_VORBIS_VORBISFILE OFF)
set(SDLMIXER_FLAC_LIBFLAC OFF)
set(SDLMIXER_VENDORED ON)
add_subdirectory(extern/SDL_mixer EXCLUDE_FROM_ALL)

set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP ON)
set(SDLIMAGE_JPG ON)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_PNG_LIBPNG OFF)
set(SDLIMAGE_VENDORED ON)
add_subdirectory(extern/SDL_image EXCLUDE_FROM_ALL)

target_link_libraries(${ENGINE_NAME} PUBLIC
	SDL3_ttf::SDL3_ttf
	SDL3_mixer::SDL3_mixer
	SDL3_image::SDL3_image
	SDL3::SDL3)

target_link_libraries(${EDITOR_NAME} PUBLIC
	${ENGINE_NAME})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${EDITOR_NAME}")

set_target_properties(freetype
					  harfbuzz
					  plutosvg
					  plutovg
					  tiff
					  SDL_uclibc
					  SDL3-shared
					  SDL3_image-shared
					  SDL3_ttf-shared
					  SDL3_mixer-shared
					  PROPERTIES FOLDER "Dependencies")
