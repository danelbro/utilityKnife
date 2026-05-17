#include "EditorApp.hpp"

#include <utl_Application.hpp>
#include <utl_StageManager.hpp>
#include <utl_SDLInterface.hpp>

#include "EditorMain.hpp"

EditorApp::EditorApp(utl::AppConfig config)
    : utl::Application{}, m_title{ config.title }, m_version{ config.version }, m_identifier{ config.identifier },
    m_screen{ config.screenSpace.w, config.screenSpace.h }, m_flags{ config.flags }, m_windowFlags{ config.windowFlags },
    m_context{ utl::create_window_with_renderer(m_title, m_screen.w, m_screen.h, m_windowFlags) },
    m_stageManager{ this }
{
    utl::init_SDL(m_title, m_version, m_identifier, m_flags);
    m_context.renderer.setVSync(1);
    LOG("Set vSync");

    m_stageManager.add_stage<EditorMain>(*this, "EditorMain");
    m_stageManager.set_current_stage("EditorMain");
    m_stageManager.set_next_stage("EditorMain");

    LOG("Created EditorApp");
}

EditorApp::~EditorApp()
{
    utl::quit_sdl();
    LOG("Destroyed EditorApp");
}

void EditorApp::trigger_stage_change([[maybe_unused]] const std::string& new_stage) {}
