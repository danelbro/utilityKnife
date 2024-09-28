#pragma once

#include "Box.hpp"
#include "SDL_Interface.hpp"
#include "StageManager.hpp"

class Application
{
public:
    virtual ~Application() { }
    virtual void run() { m_stageMan.run(); }

    virtual void trigger_stage_change(utl::StageID new_stage) = 0;
protected:
    Application(const std::string& title, int screenWidth, int screenHeight,
            uint32_t windowFlags, uint32_t renderFlags)
    :  m_title{ title }, m_screenWidth{ screenWidth },
       m_screenHeight{ screenHeight },
       m_screen{ m_screenWidth, m_screenHeight },
       m_windowFlags{ windowFlags },
       m_window{ utl::createWindow(m_title.c_str(),
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   m_screen.w, m_screen.h, m_windowFlags) },
       m_windowID{ m_window.ID() },
       m_rendererFlags{ renderFlags },
       m_renderer{ utl::createRenderer(m_window, -1, m_rendererFlags) },
       m_stageMan{ *this }
    {}

    const std::string m_title;
    const int m_screenWidth;
    const int m_screenHeight;
    const Box m_screen;
    const uint32_t m_windowFlags;
    utl::Window m_window;
    const uint32_t m_windowID;
    const uint32_t m_rendererFlags;
    utl::Renderer m_renderer;
    StageManager m_stageMan;
};
