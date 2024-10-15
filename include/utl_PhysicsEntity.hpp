#pragma once

#include "utl_Entity.hpp"
#include "utl_GameWorld.hpp"
#include "utl_PhysicsComponent.hpp"
#include "utl_SDLInterface.hpp"

#include <string>
#include <vector>

struct Vec2d;

namespace utl {

/**
 * A VecGraphPhysEnt (vector graphics physics entity) is just that: a physics
 * entity drawn with vector graphics. Not intended to be directly used.
 */
class VecGraphPhysEnt : public Entity {
public:
    VecGraphPhysEnt(const VecGraphPhysEnt&) = default;
    VecGraphPhysEnt& operator=(const VecGraphPhysEnt&) = delete;
    virtual ~VecGraphPhysEnt() = default;

    void update(double, double) override { update_shapes(); }

    /**
     * render() draws the entity using the vector graphics functions
     * in VectorDraw.hpp
     */
    void render(Renderer& renderer) override;

    /**
     * shape() returns the shape of the entity - this shape doesn’t change
     */
    const std::vector<Vec2d>& shape() const { return m_shape; }

    /**
     * collider() returns the current (rotated, translated) shape of the
     * entity - this is updated every frame
     */
    const std::vector<Vec2d>& collider() const { return m_collider; }

    bool isVisible() const { return m_isVisible; }
    bool toBeKilled() const { return m_killMe; }
    double scale() const { return m_scale; }
    const Colour& color() const { return m_color; }
    bool drawWrapped() const { return m_wrap; }

    /**
     * sets a flag intended to indicate that the entity should be destroyed
     */
    virtual void kill_it();
    void setVisible(bool vis) { m_isVisible = vis; }

    VecGraphPhysComp physicsComponent;

protected:
    VecGraphPhysEnt(const std::string& type, GameWorld& gameWorld,
                    const Vec2d& pos, const std::vector<Vec2d>& shape,
                    const Colour& color, const double& scale,
                    const double& mass, bool fill, bool wrap);
    void update_shapes();

    GameWorld& m_gameWorld;
    utl::Colour m_color;
    double m_scale;
    bool m_isVisible;
    bool m_killMe;
    bool m_fill;
    bool m_wrap;

    std::vector<Vec2d> m_shape;
    std::vector<Vec2d> m_collider;
};

/**
 * utility function: returns whether pe1 and pe2 are colliding
 */
bool areColliding(const VecGraphPhysEnt& pe1, const VecGraphPhysEnt& pe2);

}  // namespace utl
