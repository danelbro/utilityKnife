#pragma once

#include "Entity.hpp"
#include "Vec2d.hpp"

class PhysicsComponent {
public:
    PhysicsComponent(double mass, Entity* new_owner)
        : m_mass { mass }, m_impulse{ 0.0 }, m_acceleration{ 0.0, 0.0 },
          m_velocity{ 0, 0 }, m_angle{ 0.0 }, m_dir_vector{0, 0},
          m_owner { new_owner }
        {}

    void turn(double turnSpeed, double dt);

    double mass() const { return m_mass; }
    double impulse() const { return m_impulse; }
    Vec2d acceleration() const { return m_acceleration; }
    Vec2d velocity() const { return m_velocity; }
    double angle() const { return m_angle; }
    Vec2d facing() const { return m_dir_vector; }

    Entity* owner() const { return m_owner; }

    void setMass(double mass) { m_mass = mass; }
    void setFrameImpulse(double power) { m_impulse = power; }
    void setAcceleration(Vec2d accel) { m_acceleration = accel; }
    void setVelocity(Vec2d vel) { m_velocity = vel; }
    void setAngle(double angle);
    void setFacingAngle(Vec2d angle) { m_dir_vector = angle; }
    void setFacingAngle(double angle);

    void setOwner(Entity* new_owner) { m_owner = new_owner; }

    void update(double dt);

private:
    double m_mass;
    double m_impulse;
    Vec2d m_acceleration;
    Vec2d m_velocity;

    double m_angle; // between 0 and 360
    Vec2d m_dir_vector; // vector representation of m_angle

    Entity* m_owner;
};
