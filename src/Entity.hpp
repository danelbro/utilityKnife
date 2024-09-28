#pragma once

// Entity class representing an on-screen object
// to be drawn with vector graphics

#include <vector>

#include "SDL_Interface.hpp"
#include "utility.hpp"
#include "Vec2d.hpp"

struct GameWorld;

class Entity {
public:
    virtual ~Entity() = default;
    Entity(const Entity&) = default;
    Entity& operator=(const Entity&) = delete;

    virtual void update(double t, double dt) = 0;
    virtual void render(utl::Renderer& renderer) = 0;

    virtual Vec2d& pos() { return m_pos; }

    virtual const Vec2d& getPos() const { return m_pos; }
    virtual bool toBeKilled() const { return kill_me; }
    virtual double scale() const { return m_scale; }
    virtual utl::EntityFlag type() const { return m_type; };

    virtual void kill_it() { kill_me = true; }

    GameWorld& gameWorld;

protected:
    Entity(const utl::EntityFlag& new_type, GameWorld& new_gameWorld,
           const Vec2d& pos, const std::vector<Vec2d>& shape,
           const utl::Colour& color, const double& scale)
        : gameWorld{ new_gameWorld }, m_type{ new_type }, m_pos{ pos },
        m_shape{ shape }, m_color{ color }, m_scale{ scale },
        kill_me{ false }, fill{ false }
    {}

    const utl::EntityFlag m_type;
    Vec2d m_pos;
    std::vector<Vec2d> m_shape;
    utl::Colour m_color;
    double m_scale;
    bool kill_me;
    bool fill;
};
