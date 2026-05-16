add_executable(utl_Editor)
target_include_directories(utl_Editor PRIVATE "../include")
target_link_libraries(utl_Editor PRIVATE "utilityKnife" "utility_extern")
target_sources(utl_Editor PRIVATE "editor.cpp")

set(is_CC "$<CXX_COMPILER_ID:Clang,AppleClang,GNU>")
set(is_MS "$<CXX_COMPILER_ID:MSVC>")
target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<OR:${is_CC}>:-pedantic>
	$<$<OR:${is_CC}>:-pedantic-errors>
	$<$<OR:${is_CC}>:-Wall>
#	$<$<OR:${is_CC}>:-Weffc++>
	$<$<OR:${is_CC}>:-Wextra>
	$<$<OR:${is_CC}>:-Wconversion>
	$<$<OR:${is_CC}>:-Wsign-conversion>
	$<$<OR:${is_CC}>:-Werror>
	$<$<OR:${is_CC}>:-std=c++20>
	$<${is_MS}:/W4>
	$<${is_MS}:/permissive->
	$<${is_MS}:/w44365>
	$<${is_MS}:/external:W3>
  )