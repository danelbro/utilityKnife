#pragma once

#include "utl_Box.hpp"
#include "utl_SDLInterface.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <string>

namespace utl {

class Application;

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
    Stage(const Stage&) = delete;
    Stage& operator=(const Stage&) = delete;
    Stage(Stage&&) = delete;
    Stage& operator=(Stage&&) = delete;

    virtual std::string
    handle_input(std::chrono::milliseconds t, std::chrono::milliseconds dt,
                 std::array<bool, KeyFlag::K_TOTAL>& key_state) = 0;
    virtual std::string update(std::chrono::milliseconds t,
                               std::chrono::milliseconds dt) = 0;
    virtual void render(std::chrono::milliseconds t,
                        std::chrono::milliseconds dt) = 0;

    virtual Application& app() = 0;
    virtual Box& screen() = 0;
    virtual Renderer& renderer() = 0;

protected:
    Stage() = default;
};

}  // namespace utl
