#pragma once

#include <vector>

#include "Entity.hpp"
#include "PhysicsComponent.hpp"
#include "SDL_Interface.hpp"

struct GameWorld;
struct Vec2d;

class PhysicsEntity : public Entity
{
public:
    PhysicsEntity(const PhysicsEntity&) = default;
    PhysicsEntity& operator=(const PhysicsEntity&) = delete;
    virtual ~PhysicsEntity() = default;

    void update(double, double) override { update_shapes(); }
	void render(utl::Renderer& renderer) override;

	const std::vector<Vec2d>& shape() const { return m_shape; }
	const std::vector<Vec2d>& transShape() const { return m_transShape; }
	const std::vector<Vec2d>& fillShape() const { return m_fillShape; }
    const std::vector<Vec2d>& collider() const { return m_collider; }
    bool isVisible() const { return m_isVisible; }

    void setVisible(bool vis) { m_isVisible = vis; }

	PhysicsComponent physicsComponent;
protected:
    PhysicsEntity(const utl::EntityFlag& new_type, GameWorld& new_gameWorld,
                  const Vec2d& pos, const std::vector<Vec2d>& shape,
                  const utl::Colour& color, const double& scale,
                  const double& mass);

	std::vector<Vec2d> m_transShape;
	std::vector<Vec2d> m_fillShape;
    std::vector<Vec2d> m_collider;
    bool m_isVisible;

	void update_shapes();
};

namespace utl {
    bool areColliding(const PhysicsEntity& pe1, const PhysicsEntity& pe2);
} // namespace utl
