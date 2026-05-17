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
add_library(${ENGINE_NAME} STATIC)
target_sources(${ENGINE_NAME}
PRIVATE
	engine/src/utl_Application.cpp
	engine/src/utl_random.cpp
	engine/src/utl_ScoreBoard.cpp
	engine/src/utl_SDLInterface.cpp
	engine/src/utl_StageManager.cpp
	engine/src/utl_TextObject.cpp
	engine/src/utl_ValtrAlgorithm.cpp
	engine/src/utl_VecGraphPhysComp.cpp
	engine/src/utl_VecGraphPhysEnt.cpp
	engine/src/utl_VectorDraw.cpp
	engine/src/utl_Vec2d.cpp
)

target_include_directories(${ENGINE_NAME} PUBLIC engine/include)

target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)

# set up editor
set(EDITOR_NAME "utl_Editor")
add_executable(${EDITOR_NAME})
target_sources(${EDITOR_NAME}
PRIVATE
	editor/src/editor.cpp
	editor/src/EditorApp.cpp
	editor/src/EditorMain.cpp
)

target_include_directories(${EDITOR_NAME} PUBLIC editor/include)

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
