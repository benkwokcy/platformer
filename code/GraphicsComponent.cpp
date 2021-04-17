#include <stdexcept>

#include "Player.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

#include "GraphicsComponent.hpp"

GraphicsComponent::GraphicsComponent() :
    run(AnimatedSprite("assets/images/playerrun.png", 624, 58, 78, 58, 16, 28, 23, 16, 10)),
    idle(AnimatedSprite("assets/images/playeridle.png", 858, 58, 78, 58, 16, 28, 23, 16, 10)),
    attack(AnimatedSprite("assets/images/playerattack.png", 234, 58, 78, 58, 16, 28, 23, 16, 10)),
    jump(AnimatedSprite("assets/images/playerjump.png", 78, 58, 78, 58, 16, 28, 23, 16, 10)),
    fall(AnimatedSprite("assets/images/playerfall.png", 78, 58, 78, 58, 16, 28, 23, 16, 10))
{}

void GraphicsComponent::paint(Player& player) {
    auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(player.x, player.y);
    assert (!player.states.empty());
    switch (player.states.top()) {
        case PlayerState::ATTACK:
            attack.paint(screen_x, screen_y, player.facing_left);
            break;
        case PlayerState::MOVING:
            if (player.speed_y < 0.0f) {
                jump.paint(screen_x, screen_y, player.facing_left);
            } else if (player.speed_y > 0.0f && !player.on_ground) {
                fall.paint(screen_x, screen_y, player.facing_left);
            } else if (player.speed_x != 0) {
                run.paint(screen_x, screen_y, player.facing_left);
            } else if (player.speed_x == 0) {
                idle.paint(screen_x, screen_y, player.facing_left);
            } else {
                throw std::runtime_error("Unexpected state");
            }
            break;
        default:
            throw std::runtime_error("Unexpected state");
    }
}