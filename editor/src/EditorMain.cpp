#include "EditorMain.hpp"

#include <array>
#include <chrono>
#include <string>

#include <utl_Application.hpp>
#include <utl_SDLInterface.hpp>

EditorMain::EditorMain(utl::Application& app) 
	: utl::Stage{}, m_app{ &app }, m_screen{ &app.screen() }, m_renderer{ &app.renderer() }
{}

std::string EditorMain::handle_input([[maybe_unused]] std::chrono::milliseconds t, 
	[[maybe_unused]] std::chrono::milliseconds dt,
	std::array<bool, utl::KeyFlag::K_TOTAL>& key_state)
{
	utl::process_input(*m_screen, m_app->window().ID(), key_state);

	if (key_state[utl::KeyFlag::QUIT])
		return "QUIT";
	else
		return "EditorMain";
}

std::string EditorMain::update([[maybe_unused]] std::chrono::milliseconds t, 
	[[maybe_unused]] std::chrono::milliseconds dt)
{
	return "EditorMain";
}

void EditorMain::render([[maybe_unused]] std::chrono::milliseconds t, [[maybe_unused]] std::chrono::milliseconds dt)
{
	utl::clearScreen(*m_renderer);
	utl::presentRenderer(*m_renderer);
}