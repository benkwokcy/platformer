#include "Entity.hpp"
#include "GraphicsComponent.hpp"

#include "PhysicsComponent.hpp"

constexpr float GRAVITY = 0.55f;

void PhysicsComponent::tick(Entity& entity) {
    entity.speed_y += GRAVITY;
    entity.x += entity.speed_x;
    entity.y += entity.speed_y;
}
