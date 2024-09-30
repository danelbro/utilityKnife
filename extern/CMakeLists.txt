add_subdirectory(SDL)
add_subdirectory(SDL_ttf)

add_library(utility_extern INTERFACE)
target_link_libraries(utility_extern INTERFACE SDL3::SDL3)
target_link_libraries(utility_extern INTERFACE SDL2::ttf)
