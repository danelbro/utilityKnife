#pragma once

#include "utl_Application.hpp"
#include "utl_Box.hpp"
#include "utl_SDLInterface.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace utl {

/**
 * The Stage is the base class for a level, screen, stage, etc. Your derived
 * stages will need to override handle_input() - which should return an ID
 * string for the next stage (which of course could be the ID of the same
 * stage) - update() - which should probably at least call Entity::update()
 * on each of the entities that make up the level, and which should also
 * return an ID string for the next stage - and render() - which should
 * probably at least call Entity::render() on each of the entities. So
 * you’ll probably also need to include some kind of container for entities
 * as a member of your derived Stages.
 */
class Stage {
public:
    virtual ~Stage() = default;
    Stage(const Stage&) = default;
    Stage& operator=(const Stage&) = default;
    Stage(Stage&&) = default;
    Stage& operator=(Stage&&) = default;

    virtual std::string
    handle_input(double t, double dt,
                 std::array<bool, KeyFlag::K_TOTAL>& key_state) = 0;
    virtual std::string update(double t, double dt) = 0;
    virtual void render(double t, double dt) = 0;

    virtual Box& modifiable_screen() = 0;
    virtual const Box& screen() const = 0;
    virtual uint32_t windowID() = 0;
    virtual Renderer& renderer() = 0;
    virtual const std::string& ID() const = 0;
    virtual const Application& app() const = 0;

protected:
    Stage();
};

}  // namespace utl
