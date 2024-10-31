#include "utl_VecGraphPhysEnt.hpp"

#include "utl_Entity.hpp"
#include "utl_GameWorld.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"
#include "utl_VecGraphPhysComp.hpp"
#include "utl_VectorDraw.hpp"

#include <algorithm>
#include <vector>

namespace utl {

VecGraphPhysEnt::VecGraphPhysEnt(const std::string& type, GameWorld& gameWorld,
                                 const Vec2d& pos,
                                 const std::vector<Vec2d>& shape,
                                 const Colour& color, const double& scale,
                                 const double& mass, bool wrap, bool fill)
    : Entity{type, gameWorld.screen, pos}, physicsComponent{mass, this},
      m_gameWorld{gameWorld}, m_color{color}, m_scale{scale}, m_isVisible{true},
      m_killMe{false}, m_wrap{wrap}, m_fill{fill}, m_shape{shape},
      m_rotatedShape{}, m_collider{}
{
    update_shapes();
}

void VecGraphPhysEnt::update_shapes()
{
    m_collider.clear();
    m_collider.reserve(m_shape.size());

    for (auto p : m_shape) {
        p = p.rotate_deg(physicsComponent.facing());
        p = p * m_scale;
        p += m_pos;
        m_collider.emplace_back(p);
    }
}

void VecGraphPhysEnt::render(Renderer& renderer)
{
    if (!m_isVisible) {
        return;
    }

    auto oldColor{getRendererDrawColour(renderer)};
    setRendererDrawColour(renderer, m_color);

    const size_t& colliderSize{m_collider.size()};
    for (size_t i{0}; i < colliderSize; ++i) {
        DrawWrapLine(renderer, m_screenSpace, m_collider[i].x, m_collider[i].y,
                     m_collider[(i + 1) % colliderSize].x,
                     m_collider[(i + 1) % colliderSize].y);
    }

    if (m_fill) {
        ScanFill(m_screenSpace, m_collider, m_color, renderer);
    }

    setRendererDrawColour(renderer, oldColor);
}

static std::vector<Vec2d> syncColliderWorldSpace(const VecGraphPhysEnt& pe1,
                                                 const VecGraphPhysEnt& pe2)
{
    // we're going to sync pe1 to pe2
    auto size{pe2.collider().size()};
    std::vector<double> pe2ColliderXs{};
    std::vector<double> pe2ColliderYs{};
    double screenXMax{static_cast<double>(pe2.screen().w)};
    double screenYMax{static_cast<double>(pe2.screen().h)};
    Vec2d pos{pe1.pos()};

    pe2ColliderXs.reserve(size);
    pe2ColliderYs.reserve(size);

    for (const auto& point : pe2.collider()) {
        pe2ColliderXs.emplace_back(point.x);
        pe2ColliderYs.emplace_back(point.y);
    }

    std::ranges::sort(pe2ColliderXs);
    std::ranges::sort(pe2ColliderYs);

    double xMin{*pe2ColliderXs.begin()};
    double xMax{*pe2ColliderXs.end()};

    double yMin{*pe2ColliderYs.begin()};
    double yMax{*pe2ColliderYs.end()};

    if (xMin >= 0 && xMax <= screenXMax && yMin >= 0 && yMin <= screenYMax)
        return pe1.collider();

    if (xMin < 0) {
        if (pos.x > screenXMax + xMin) {
            pos.x -= screenXMax;
        }
    } else if (xMax > screenXMax) {
        if (pos.x < xMax - screenXMax) {
            pos.x += screenXMax;
        }
    }

    if (yMin < 0) {
        if (pos.y > screenYMax + yMax) {
            pos.y -= screenYMax;
        }
    } else if (yMax > screenYMax) {
        if (pos.y < yMax - screenYMax) {
            pos.y += screenYMax;
        }
    }

    std::vector<Vec2d> pe1ColliderSynced{};
    for (auto p : pe1.shape()) {
        p = p.rotate_deg(pe1.physicsComponent.facing());
        p = p * pe1.scale();
        p += pos;
        pe1ColliderSynced.emplace_back(p);
    }

    return pe1ColliderSynced;
}

Vec2d syncPointToColliderWorldSpace(const VecGraphPhysEnt& pe1,
                                    const VecGraphPhysEnt& pe2)
{
    // we're going to sync pe1 to pe2
    auto size{pe2.collider().size()};
    std::vector<double> pe2ColliderXs{};
    std::vector<double> pe2ColliderYs{};
    double screenXMax{static_cast<double>(pe2.screen().w)};
    double screenYMax{static_cast<double>(pe2.screen().h)};
    Vec2d pos{pe1.pos()};

    pe2ColliderXs.reserve(size);
    pe2ColliderYs.reserve(size);

    for (const auto& point : pe2.collider()) {
        pe2ColliderXs.emplace_back(point.x);
        pe2ColliderYs.emplace_back(point.y);
    }

    std::ranges::sort(pe2ColliderXs);
    std::ranges::sort(pe2ColliderYs);

    double xMin{*pe2ColliderXs.begin()};
    double xMax{*pe2ColliderXs.end()};

    double yMin{*pe2ColliderYs.begin()};
    double yMax{*pe2ColliderYs.end()};

    if (xMin >= 0 && xMax <= screenXMax && yMin >= 0 && yMin <= screenYMax)
        return pos;

    if (xMin < 0) {
        if (pos.x > screenXMax + xMin) {
            pos.x -= screenXMax;
        }
    } else if (xMax > screenXMax) {
        if (pos.x < xMax - screenXMax) {
            pos.x += screenXMax;
        }
    }

    if (yMin < 0) {
        if (pos.y > screenYMax + yMax) {
            pos.y -= screenYMax;
        }
    } else if (yMax > screenYMax) {
        if (pos.y < yMax - screenYMax) {
            pos.y += screenYMax;
        }
    }

    return pos;
}

bool isPointInPolygonSyncFirst(const VecGraphPhysEnt& pe1,
                               const VecGraphPhysEnt& pe2)
{
    Vec2d point{syncPointToColliderWorldSpace(pe1, pe2)};

    return isPointInPolygon(point, pe2.collider());
}

bool areColliding(const VecGraphPhysEnt& pe1, const VecGraphPhysEnt& pe2)
{
    std::vector<Vec2d> pe1Collider{syncColliderWorldSpace(pe1, pe2)};

    // from VectorDraw.hpp
    return areColliding_SAT(pe1Collider, pe2.collider());
}

}  // namespace utl
