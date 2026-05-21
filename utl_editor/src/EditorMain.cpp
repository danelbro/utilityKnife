#include "EditorMain.hpp"

#include <array>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <string>

#include "utl_TextObject.hpp"
#include <utl_Application.hpp>
#include <utl_SDLInterface.hpp>

EditorMain::EditorMain(utl::Application& app)
    : utl::Stage{}, m_app{&app}, m_screen{&app.screen()},
      m_renderer{&app.renderer()}
{
    auto fontPath = std::filesystem::path{"data/Silkscreen.ttf"};
    fonts.at(0) = utl::Font{utl::createFont(fontPath, 72)};
    auto text = std::make_unique<utl::TextObject>(
        this, fonts.data(), utl::Colour{0xff, 0xff, 0xff, 0xff}, "utl_Editor");
    text->set_pos({0.0, m_screen->w / 2.0});
    entities.emplace_back(std::move(text));
}

std::string
EditorMain::handle_input([[maybe_unused]] std::chrono::milliseconds t,
                         [[maybe_unused]] std::chrono::milliseconds dt,
                         std::array<bool, utl::KeyFlag::K_TOTAL>& key_state)
{
    utl::process_input(*m_screen, m_app->window().ID(), key_state);

    if (key_state[utl::KeyFlag::QUIT] || key_state[utl::KeyFlag::K_ESCAPE])
        return "QUIT";
    else
        return "EditorMain";
}

static void updateTitle(std::vector<std::unique_ptr<utl::Entity>>& entities,
                        utl::Box& screen, std::chrono::milliseconds t)
{
    const double damping{0.25};
    const double omega{0.0005};
    for (auto& entity : entities) {
        if (!entity)
            continue;
        try {
            auto& to = dynamic_cast<utl::TextObject&>(*entity);
            to.set_x_pos(
                static_cast<double>(
                    static_cast<int>(damping * static_cast<int>(t.count()))
                    % (screen.w + static_cast<int>(to.size().w)))
                - to.size().w);
            to.set_y_pos(
                ((1 + std::sin(omega * static_cast<double>(t.count()))) / 2)
                * (screen.h - to.size().h));
        }
        catch (std::bad_cast&) {
            continue;
        }
    }
}

std::string EditorMain::update([[maybe_unused]] std::chrono::milliseconds t,
                               [[maybe_unused]] std::chrono::milliseconds dt)
{
    for (auto& entity : entities) {
        entity->update(t, dt);
    }

    updateTitle(entities, *m_screen, t);
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
