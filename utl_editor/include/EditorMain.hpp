#pragma once

#include <array>
#include <chrono>
#include <string>

#include <utl_Application.hpp>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_Stage.hpp>
#include <utl_SDLInterface.hpp>

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
    std::vector<std::unique_ptr<utl::Entity>> entities{};
};

