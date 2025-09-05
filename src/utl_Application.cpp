#include "utl_Application.hpp"

#include "utl_SDLInterface.hpp"

namespace utl {

Application::Application()
    // : isSDLInitialised{init_SDL(title, version, identifier, flags)},
    //   m_title{title}, m_screenWidth{screenWidth}, m_screenHeight{screenHeight},
    //   m_screenSpace{m_screenWidth, m_screenHeight}, m_windowFlags{windowFlags},
    //   m_windowRendStruct(create_window_with_renderer(
    //       m_title, m_screenWidth, m_screenHeight, windowFlags)),
    //   m_window{std::move(m_windowRendStruct.window)}, m_windowID{m_window.ID()},
    //   m_renderer{std::move(m_windowRendStruct.renderer)}, m_stageManager{*this}
{
    LOG("Constructed Application\n");
}

Application::~Application()
{
    quit_sdl();
    LOG("Destroying Application\n");
}

void Application::run()
{
    stageManager().run();
}

}  // namespace utl
