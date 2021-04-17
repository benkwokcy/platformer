#include "Entity.hpp"
#include "GraphicsComponent.hpp"

#include "PhysicsComponent.hpp"

constexpr float GRAVITY = 0.55f;

void PhysicsComponent::tick(Entity& entity) {
    if (Input::left_down() || Input::right_down()) {
        if (entity.facing_left) {
            entity.speed_x = -4.0f;
        } else {
            entity.speed_x = 4.0f;
        }
    } else {
        entity.speed_x = 0.0f;
    }
    entity.on_ground = false; // assume we are not on the ground and let the collision logic tell us if we are.
    entity.speed_y += GRAVITY;
    entity.x += entity.speed_x;
    entity.y += entity.speed_y;
    if (entity.states.top() == EntityState::ATTACK && entity.graphics->attack.animation_complete()) {
        entity.states.pop();
    }
}
