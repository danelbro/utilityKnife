#pragma once

#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <chrono>
#include <string>

namespace utl {

struct Size {
    double w;
    double h;
};

class Stage;

/**
 * The Entity is the base class for any kind of /thing/ in the game. It has a
 * type and a position, and holds a reference to the screen space - and not
 * much else.
 */
class Entity {
public:
    virtual ~Entity() = default;

    /**
     * update() is called each frame. It should update the Entity’s state in
     * response to input, physics, etc.
     *
     * It’s recommended to derive from one of the provided derived classes
     * instead of Entity as they will handle the relevant updates.
     */
    virtual void update(std::chrono::milliseconds t,
                        std::chrono::milliseconds dt) = 0;

    /**
     * render() is called each frame. It should draw the Entity to the screen.
     *
     * It’s recommended to derive from one of the provided derived classes
     * instead of Entity as they will handle drawing.
     */
    virtual void render(Renderer& renderer) = 0;

    virtual const std::string& type() const = 0;
    virtual const Vec2d& pos() const = 0;
    virtual const Size& size() const = 0;
    virtual Stage& stage() = 0;

    virtual void set_pos(const Vec2d& new_pos) = 0;

protected:
    Entity() = default;
    Entity(const Entity&) = default;
    Entity& operator=(const Entity&) = default;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
};

}  // namespace utl
