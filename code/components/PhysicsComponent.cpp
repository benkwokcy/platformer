#include "Player.hpp"
#include "GraphicsComponent.hpp"

#include "PhysicsComponent.hpp"

constexpr float GRAVITY = 0.55f;

void PhysicsComponent::tick(Player& player) {
    if (Input::left_down() || Input::right_down()) {
        if (player.facing_left) {
            player.speed_x = -4.0f;
        } else {
            player.speed_x = 4.0f;
        }
    } else {
        player.speed_x = 0.0f;
    }
    player.on_ground = false; // assume we are not on the ground and let the collision logic tell us if we are.
    player.speed_y += GRAVITY;
    player.x += player.speed_x;
    player.y += player.speed_y;
    if (player.states.top() == PlayerState::ATTACK && player.graphics->attack.animation_complete()) {
        player.states.pop();
    }
}
