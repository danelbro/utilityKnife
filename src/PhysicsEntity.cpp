#include "PhysicsEntity.hpp"

#include <vector>

#include "SDL_Interface.hpp"
#include "Entity.hpp"
#include "GameWorld.hpp"
#include "PhysicsComponent.hpp"
#include "Vec2d.hpp"
#include "VectorDraw.hpp"

namespace utl {

VecGraphPhysEnt::VecGraphPhysEnt(const std::string& type, const GameWorld& gameWorld, const Vec2d& pos, const std::vector<Vec2d>& shape,
                                 const Colour& color, const double& scale, const double& mass, bool fill, bool wrap)
    : Entity{ type, gameWorld.screen, pos }, physicsComponent{ mass, this }, m_gameWorld{ gameWorld }, m_color{ color }, m_scale { scale },
      m_isVisible{ true }, m_killMe{ false }, m_fill{ fill }, m_wrap { wrap }, m_shape{ shape }, m_collider{}
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

    auto oldColor{ getRendererDrawColour(renderer) };
    setRendererDrawColour(renderer, m_color);

    const size_t& colliderSize{ m_collider.size() };
    for (size_t i{ 0 }; i < colliderSize; ++i) {
        DrawWrapLine(renderer, screenSpace, m_collider[i].x, m_collider[i].y, m_collider[(i + 1) % colliderSize].x, m_collider[(i + 1) % colliderSize].y);
    }

    if (m_fill) {
        ScanFill(screenSpace, m_collider, m_color, renderer);
    }

    setRendererDrawColour(renderer, oldColor);
}

bool areColliding(const VecGraphPhysEnt &pe1, const VecGraphPhysEnt &pe2)
{
    // from VectorDraw.hpp
    return areColliding_SAT(pe1.collider(), pe2.collider());
}

} // namespace utl
