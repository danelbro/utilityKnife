#pragma once


#include <array>
#include <cstdint>
#include <string>

#include "SDL_Interface.hpp"

#include "Box.hpp"

namespace utl {

/**
 * The Stage is the base class for a level, screen, stage, etc. Your derived
 * stages will need to override handle_input() - which should return an ID
 * string for the next stage (which of course could be the ID of the same
 * stage ) - update() - which should probably at least call Entity::update()
 * on each of the entities that make up the level, and which should also
 * return an ID string for the next stage - and render() - which should
 * probably at least call Entity::render() on each of the entities. So
 * you’ll probably also need to include some kind of container for entities
 * as a member of your derived Stages.
 */
class Stage {
public:
    Stage(const Stage&) = default;
    Stage& operator=(const Stage&) = delete;
    virtual ~Stage() = default;

    virtual std::string handle_input(double t, double dt,
        std::array<bool, KeyFlag::K_TOTAL>& key_state) = 0;
    virtual std::string update(double t, double dt) = 0;
    virtual void render(double t, double dt) = 0;

    Box screen() const { return m_screen; }
    uint32_t windowID() const { return m_windowID; }
    Renderer& renderer() { return m_rend; }
    std::string ID() const { return m_ID; }
protected:
    Stage(const Box& screen, uint32_t windowID, Renderer& renderer,
          const std::string& id)
        : m_screen{ screen }, m_windowID{ windowID },
        m_rend{ renderer }, m_ID{ id }
        {}
private:
    Box m_screen;
    uint32_t m_windowID;
    Renderer& m_rend;
    std::string m_ID;
};

} // namespace utl
