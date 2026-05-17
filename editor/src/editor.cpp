#include "EditorApp.hpp"

#include <SDL3/SDL_main.h>

#include <utl_Application.hpp>


int main(int, char **)
{
    utl::AppConfig config{ "UtilityKnife Editor", "0.0.1", "com.utilityKnife.editor",
                            {1280, 720}, SDL_INIT_VIDEO | SDL_INIT_AUDIO, SDL_WINDOW_RESIZABLE };
    EditorApp editor{config};
    editor.run();
    return 0;
}