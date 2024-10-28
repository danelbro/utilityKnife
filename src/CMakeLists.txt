add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE
    "../include/utl_Application.hpp" “utl_Application.cpp”
    "../include/utl_Box.hpp"
    "../include/utl_Entity.hpp"
    "../include/utl_GameWorld.hpp"
    "../include/utl_VecGraphPhysComp.hpp" "utl_VecGraphyPhysComp.cpp"
    "../include/utl_VecGraphPhysEnt.hpp" "utl_VecGraphPhysEnt.cpp"
    "../include/utl_ScoreBoard.hpp" "utl_ScoreBoard.cpp"
    "../include/utl_SDLInterface.hpp" "utl_SDLInterface.cpp"
    "../include/utl_Stage.hpp"
    "../include/utl_StageManager.hpp" "utl_StageManager.cpp"
    "../include/utl_TextObject.hpp" "utl_TextObject.cpp"
    "../include/utl_random.hpp" "utl_random.cpp"
    "../include/utl_ValtrAlgorithm.hpp" "utl_ValtrAlgorithm.cpp"
    "../include/utl_Vec2d.hpp" "utl_Vec2d.cpp"
    "../include/utl_VectorDraw.hpp" "utl_VectorDraw.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC utility_extern)
target_include_directories(${PROJECT_NAME} PUBLIC "../include")
set(is_Clang "$<CXX_COMPILER_ID:Clang,AppleClang>")
set(is_CC    "$<CXX_COMPILER_ID:Clang,AppleClang,GNU>")
set(is_MS "$<CXX_COMPILER_ID:MSVC>")
target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<OR:${is_CC}>:-Wall>
	$<$<OR:${is_CC}>:-Wextra>
	$<$<OR:${is_CC}>:-Wsign-conversion>
	$<$<OR:${is_CC}>:-pedantic>
	$<$<OR:${is_CC}>:-std=c++20>
	$<$<OR:${is_CC}>:-Weffc++>
    	$<$<OR:${is_Clang}>:-x c++>
	$<${is_MS}:/W4>)
