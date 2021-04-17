#include "Entity.hpp"
#include "GraphicsComponent.hpp"

#include "PhysicsComponent.hpp"

constexpr float GRAVITY = 0.55f;

void PhysicsComponent::tick(Entity& entity) {
    entity.on_ground = false; // assume we are not on the ground and let the collision logic tell us if we are.
    entity.speed_y += GRAVITY;
    entity.x += entity.speed_x;
    entity.y += entity.speed_y;
}
