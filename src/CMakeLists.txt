add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE
    Application.hpp
    Box.hpp
    Entity.hpp
    GameWorld.hpp
    PhysicsComponent.hpp PhysicsComponent.cpp
    PhysicsComponent.hpp PhysicsEntity.cpp
    SDL_Interface.hpp SDL_Interface.cpp
    Stage.hpp
    StageManager.hpp StageManager.cpp
    TextObject.hpp TextObject.cpp
    utility.hpp utility.cpp
    ValtrAlgorithm.hpp ValtrAlgorithm.cpp
    Vec2d.hpp Vec2d.cpp
    VectorDraw.hpp VectorDraw.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE utility_extern)
target_include_directories(${PROJECT_NAME} PUBLIC ./)
set(is_CC "$<CXX_COMPILER_ID:Clang,AppleClang,GNU>")
set(is_MS "$<CXX_COMPILER_ID:MSVC>")
target_compile_options(${PROJECT_NAME} PRIVATE 
	$<$<OR:${is_CC}>:-Wall>
	$<$<OR:${is_CC}>:-Wextra>
	$<$<OR:${is_CC}>:-Wsign-conversion>
	$<$<OR:${is_CC}>:-pedantic>
	$<$<OR:${is_CC}>:-std=c++20>
	$<$<OR:${is_CC}>:-Weffc++>
	$<${is_MS}:/W4>)