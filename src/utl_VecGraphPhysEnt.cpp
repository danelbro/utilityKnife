#include "utl_VecGraphPhysEnt.hpp"

#include "utl_Entity.hpp"
#include "utl_GameWorld.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"
#include "utl_VecGraphPhysComp.hpp"
#include "utl_VectorDraw.hpp"

#include <vector>

namespace utl {

VecGraphPhysEnt::VecGraphPhysEnt(const std::string& type, GameWorld& gameWorld,
                                 const Vec2d& pos,
                                 const std::vector<Vec2d>& shape,
                                 const Colour& color, const double& scale,
                                 const double& mass, bool wrap, bool fill)
    : Entity{type, gameWorld.screen, pos}, physicsComponent{mass, this},
      m_gameWorld{gameWorld}, m_color{color}, m_scale{scale}, m_isVisible{true},
      m_killMe{false}, m_wrap{wrap}, m_fill{fill}, m_shape{shape}, m_rotatedShape{}, 
      m_collider{}
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

bool areColliding(const VecGraphPhysEnt& pe1, const VecGraphPhysEnt& pe2)
{
    // from VectorDraw.hpp
    return areColliding_SAT(pe1.collider(), pe2.collider());
}

}  // namespace utl
