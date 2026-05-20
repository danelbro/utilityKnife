#include "EditorMain.hpp"

#include <array>
#include <chrono>
#include <filesystem>
#include <string>

#include "utl_TextObject.hpp"
#include "utl_Vec2d.hpp"
#include <utl_Application.hpp>
#include <utl_SDLInterface.hpp>

EditorMain::EditorMain(utl::Application& app)
    : utl::Stage{}, m_app{&app}, m_screen{&app.screen()},
      m_renderer{&app.renderer()}
{
    auto fontPath = std::filesystem::path{"data/Silkscreen.ttf"};
    auto font = std::make_shared<utl::Font>(utl::createFont(fontPath, 72));
    auto text = std::make_unique<utl::TextObject>(
        this, font.get(), utl::Colour{0xff, 0xff, 0xff, 0xff}, "utl_Editor",
        utl::Vec2d{400.0, 300.0});
    entities.emplace_back(std::move(text));
}

std::string
EditorMain::handle_input([[maybe_unused]] std::chrono::milliseconds t,
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

void EditorMain::render([[maybe_unused]] std::chrono::milliseconds t,
                        [[maybe_unused]] std::chrono::milliseconds dt)
{
    utl::clearScreen(*m_renderer);
    for (auto& entity : entities) {
        entity->render(*m_renderer);
    }
    utl::presentRenderer(*m_renderer);
}
