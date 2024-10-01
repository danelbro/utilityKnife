#pragma once

#include <string>

#include "Box.hpp"
#include "SDL_Interface.hpp"
#include "StageManager.hpp"

class Application
{
public:
    virtual ~Application() { }
    virtual void run() { m_stageMan.run(); }

    virtual void trigger_stage_change(const std::string& new_stage) = 0;
protected:
    Application(const std::string& title, int screenWidth, int screenHeight, uint32_t flags)
    :  m_title{ title }, m_screenWidth{ screenWidth },
       m_screenHeight{ screenHeight },
       m_screen{ m_screenWidth, m_screenHeight },
       m_windowFlags{ flags },
       m_window{ utl::createWindow(m_title.c_str(), m_screen.w, m_screen.h, m_windowFlags) },
       m_windowID{ m_window.ID() },
       m_renderer{ utl::createRenderer(m_window, NULL) },
       m_stageMan{ *this }
    {}

    const std::string m_title;
    const int m_screenWidth;
    const int m_screenHeight;
    const Box m_screen;
    const uint32_t m_windowFlags;
    utl::Window m_window;
    const uint32_t m_windowID;
    utl::Renderer m_renderer;
    StageManager m_stageMan;
};
