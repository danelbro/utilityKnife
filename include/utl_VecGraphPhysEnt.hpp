#pragma once

#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"
#include "utl_VecGraphPhysComp.hpp"

#include <string>
#include <vector>

namespace utl {

class Stage;
struct Vec2d;

struct VecGraphPhysEntConfig {
    const Colour color;
    const double scale;
    const double mass;
    const bool wrap;
    const bool fill;
};

/**
 * A VecGraphPhysEnt (vector graphics physics entity) is just that: a physics
 * entity drawn with vector graphics. Not intended to be directly used.
 */
class VecGraphPhysEnt : public Entity {
public:
    void update(double, double) override;

    /**
     * render() draws the entity using the vector graphics functions
     * in VectorDraw.hpp
     */
    void render(Renderer& renderer) override;
    const std::string& type() const override { return m_type; }
    const Vec2d& pos() const override { return m_pos; }
    const Size& size() const override { return m_size; };
    const Stage& stage() const override { return *m_stage; }
    void set_pos(const Vec2d& new_pos) override;

    const std::vector<Vec2d>& rotatedShape() const { return m_rotatedShape; }

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
    virtual void kill_it() { m_killMe = true; };
    void setVisible(bool vis) { m_isVisible = vis; }

public:
    VecGraphPhysComp physicsComponent;
    const std::vector<Vec2d> shape;

protected:
    VecGraphPhysEnt(const Stage* stage, const std::string& type,
                    const Vec2d& pos, const std::vector<Vec2d>& shape,
                    const VecGraphPhysEntConfig& config);
    void update_shapes();

    const std::string m_type;
    Vec2d m_pos;
    const Stage* m_stage;

    Colour m_color;
    double m_scale;
    bool m_isVisible;
    bool m_killMe;
    bool m_wrap;
    bool m_fill;

    std::vector<Vec2d> m_rotatedShape;
    std::vector<Vec2d> m_collider;

    Size m_size = {0,0};  // todo
};

/**
 * utility function: returns whether pe1 and pe2 are colliding
 */
bool isPointInPolygonSyncFirst(const VecGraphPhysEnt& pe1,
                               const VecGraphPhysEnt& pe2, const Box& screen);
bool areColliding(const VecGraphPhysEnt& pe1, const VecGraphPhysEnt& pe2,
                  const Box& screen);

}  // namespace utl
