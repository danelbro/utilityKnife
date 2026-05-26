#pragma once

#include <array>
#include <chrono>
#include <string>
#include <unordered_map>

#include <utl_Application.hpp>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_Stage.hpp>
#include <utl_SDLInterface.hpp>
#include <vector>
#include "flags.hpp"

class EditorMain : public utl::Stage
{
public:
    EditorMain() = default;
    EditorMain(utl::Application& app);

    std::string handle_input(std::chrono::milliseconds t, std::chrono::milliseconds dt,
        std::array<bool, utl::KeyFlag::K_TOTAL>& key_state) final;

    std::string update(std::chrono::milliseconds t, std::chrono::milliseconds dt) final;

    void render(std::chrono::milliseconds t, std::chrono::milliseconds dt) final;

    utl::Application& app() final { return *m_app; }
    utl::Box& screen() final { return *m_screen; }
    utl::Renderer& renderer() final { return *m_renderer; }
private:
    utl::Application* m_app{};
    utl::Box* m_screen{};
    utl::Renderer* m_renderer{};
    utl::Mixer m_mixer{};
    std::unordered_map<size_t, utl::Track> m_tracks;
    size_t musicTrackID{};
    size_t effectsTrackID{};
    std::vector<utl::Effect> m_effects{};
    utl::Music m_music{};
    utl::EntityPool entities{};
    std::unordered_map<utl::Fonts, utl::Font> fonts{};
    std::unordered_map<size_t, size_t> entityMap{};
    size_t titleID{};
    bool popped1{false};
    bool popped2{false};
    bool popped3{false};
};
