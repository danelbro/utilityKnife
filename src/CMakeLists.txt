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
