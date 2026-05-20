#include <utl_SDLInterface.hpp>
#include <utl_Application.hpp>
#include <SDL3/SDL_main.h>

#include "EditorApp.hpp"

int main(int, char**)
{
    try {
        utl::AppConfig config{
            "UtilityKnife Editor",           "0.0.1",
            "com.utilityKnife.editor",       {1280, 720},
            SDL_INIT_VIDEO | SDL_INIT_AUDIO, SDL_WINDOW_RESIZABLE};
        EditorApp editor{config};
        editor.run();
    }
    catch (utl::SdlException& e) {
        ERRLOGF("%s\n", e.what());
        return 1;
    }
    return 0;
}
