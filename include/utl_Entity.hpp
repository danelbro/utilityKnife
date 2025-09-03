#pragma once

#include "utl_Box.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <string>

namespace utl {

/**
 * The Entity is the base class for any kind of /thing/ in the game. It has a
 * type and a position, and holds a reference to the screen space - and not
 * much else.
 */
class Entity {
public:
    virtual ~Entity() = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = delete;

    /**
     * update() is called each frame. It should update the Entity’s state in
     * response to input, physics, etc.
     *
     * It’s recommended to derive from one of the provided derived classes
     * instead of Entity as they will handle the relevant updates.
     */
    virtual void update([[maybe_unused]] double t, [[maybe_unused]] double dt) {
    };

    /**
     * render() is called each frame. It should draw the Entity to the screen.
     *
     * It’s recommended to derive from one of the provided derived classes
     * instead of Entity as they will handle drawing.
     */
    virtual void render([[maybe_unused]] Renderer& renderer) {};
    virtual Vec2d size() const { return {}; };

    const Box& screen() const { return m_screenSpace; }
    std::string type() const { return m_type; }
    const Vec2d& pos() const { return m_pos; }

    void changeScreen(Box& newScreen) { m_screenSpace = newScreen; }
    void updateScreen(const Box& newScreenSpace)
    {
        m_screenSpace = newScreenSpace;
    }
    void set_pos(const Vec2d& newPos) { m_pos = newPos; }

protected:
    Entity(const std::string& new_type, Box& screen, const Vec2d& pos)
        : m_screenSpace{screen}, m_type{new_type}, m_pos{pos}
    {}

    Box& m_screenSpace;
    const std::string m_type;
    Vec2d m_pos;
};

}  // namespace utl
