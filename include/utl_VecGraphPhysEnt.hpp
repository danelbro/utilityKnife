#pragma once

#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"
#include "utl_VecGraphPhysComp.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace utl {

class Stage;
struct Vec2d;

struct VecGraphPhysEntConfig {
    const std::string type;
    const std::vector<Vec2d> shape;
    const Vec2d pos;
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
    void update(std::chrono::milliseconds, std::chrono::milliseconds) override;

    /**
     * render() draws the entity using the vector graphics functions
     * in VectorDraw.hpp
     */
    void render(Renderer& renderer) override;
    const std::string& type() const override;
    const Vec2d& pos() const override;
    const Size& size() const override;
    Stage& stage() override;
    void set_pos(const Vec2d& new_pos) override;

    const std::vector<Vec2d>& rotatedShape() const;

    /**
     * collider() returns the current (rotated, translated) shape of the
     * entity - this is updated every frame
     */
    const std::vector<Vec2d>& collider() const;

    bool isVisible() const;
    bool toBeKilled() const;
    double scale() const;
    const Colour& color() const;
    bool drawWrapped() const;

    /**
     * sets a flag intended to indicate that the entity should be destroyed
     */
    virtual void kill_it();
    void setVisible(bool vis);

public:
    VecGraphPhysComp physicsComponent{0.0, this};
    const std::vector<Vec2d> shape{};

protected:
    VecGraphPhysEnt() = default;
    VecGraphPhysEnt(Stage* stage, const VecGraphPhysEntConfig& config);

    void update_shapes();

protected:
    Stage* m_stage{nullptr};
    std::string m_type{"VECTORGRAPHICS_PHYSICSENTITY"};
    Vec2d m_pos{};
    Size m_size{};

    Colour m_color{0xff, 0xff, 0xff, 0xff};
    double m_scale{1.0};
    bool m_isVisible{true};
    bool m_killMe{false};
    bool m_wrap{true};
    bool m_fill{false};

    std::vector<Vec2d> m_rotatedShape{};
    std::vector<Vec2d> m_collider{};
};

/**
 * utility function: returns whether pe1 and pe2 are colliding
 */
bool isPointInPolygonSyncFirst(const VecGraphPhysEnt& pe1,
                               const VecGraphPhysEnt& pe2, const Box& screen);
bool areColliding(const VecGraphPhysEnt& pe1, const VecGraphPhysEnt& pe2,
                  const Box& screen);

}  // namespace utl
