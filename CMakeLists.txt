cmake_minimum_required(VERSION 3.28)

project(utilityKnife)

option(UTILITYKNIFE_VENDORED "Use vendored libraries" ON)

if(UTILITYKNIFE_VENDORED)
    add_subdirectory(../SDL build)
else()
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()

add_library(${CMAKE_PROJECT_NAME} 
    src/Application.hpp
    src/Box.hpp
    src/Entity.hpp
    src/GameWorld.hpp
    src/PhysicsComponent.hpp src/PhysicsComponent.cpp 
    src/PhysicsComponent.hpp src/PhysicsEntity.cpp 
    src/SDL_Interface.hpp src/SDL_Interface.cpp 
    src/Stage.hpp
    src/StageManager.hpp src/StageManager.cpp 
    src/TextObject.hpp src/TextObject.cpp 
    src/utility.hpp src/utility.cpp 
    src/ValtrAlgorithm.hpp src/ValtrAlgorithm.cpp 
    src/Vec2d.hpp src/Vec2d.cpp 
    src/VectorDraw.hpp src/VectorDraw.cpp)

if(TARGET SDL2::SDL2main)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2)
