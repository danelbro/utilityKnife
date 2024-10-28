#pragma once

#include "utl_Box.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_StageManager.hpp"

#include <string>

namespace utl {

/**
 *
 * Application is the base class from which your app should derive. When writing
 * your derived app class, you are responsible for:
 *
 * - in your app’s constructor, adding at least one (derived) Stage to your
 * app’s StageManager by calling m_stageManager.add_stage(), and setting it as
 * the current stage by calling m_stageManager.set_current_stage(). Your derived
 * stages will contain all of your app’s logic and functionality.
 *
 * - setting the Renderer’s behaviour [currently, all you can do is
 *   m_renderer.setVSync()].
 *
 * - overriding trigger_stage_change(). The StageManager calls this function
 * when a Stage returns a non-current Stage ID string from its update() or
 * handle_input() functions, passing in the new Stage ID string. In response to
 * the new Stage ID, in this function you should add a stage of that type to the
 * StageManager by calling m_stageManager.add_stage(). A switch statement is
 * recommended! Make sure to handle StageID::QUIT - you can call add_stage with
 * a nullptr - and note that you can put any behaviour you want on stage
 * transitions here.
 *
 * - instantiating a derived Application class (use RAII!). You’ll need to
 * decide on at least a title and screen size (width and height), as well as any
 * SDL window flags you want.
 *
 * - in main(), calling yourApp.run() (remember to #include <SDL3/SDL_main.h>,
 *   particularly if you’re on Windows).
 *
 */
class Application {
public:
    virtual ~Application();
    virtual void run();

    virtual void trigger_stage_change(const std::string& new_stage) = 0;

protected:
    Application(const std::string& title, int screenWidth, int screenHeight,
                uint32_t flags, uint32_t windowFlags);

    bool isSDLInitialised;
    const std::string m_title;
    const int m_screenWidth;
    const int m_screenHeight;
    Box m_screenSpace;
    const uint32_t m_windowFlags;
    WindowWithRenderer m_windowRendStruct;
    Window m_window;
    const uint32_t m_windowID;
    Renderer m_renderer;
    StageManager m_stageManager;
};

}  // namespace utl
