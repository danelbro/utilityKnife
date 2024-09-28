#pragma once

#include <array>
#include <cstdint>

#include "SDL_Interface.hpp"

#include "Box.hpp"
#include "utility.hpp"

class Stage {
public:
    Stage(const Stage&) = default;
    Stage& operator=(const Stage&) = delete;
    virtual ~Stage() = default;

    virtual utl::StageID handle_input(double t, double dt,
        std::array<bool, utl::KeyFlag::K_TOTAL>& key_state) = 0;
    virtual utl::StageID update(double t, double dt) = 0;
    virtual void render(double t, double dt) = 0;

    Box screen() const { return m_screen; }
    uint32_t windowID() const { return m_windowID; }
    utl::Renderer& renderer() { return m_rend; }
    utl::StageID ID() const { return m_ID; }
protected:
    Stage(const Box& screen, uint32_t windowID, utl::Renderer& renderer,
          const utl::StageID& id)
        : m_screen{ screen }, m_windowID{ windowID },
        m_rend{ renderer }, m_ID{ id }
        {}
private:
    Box m_screen;
    uint32_t m_windowID;
    utl::Renderer& m_rend;
    utl::StageID m_ID;
};
