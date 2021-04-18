#include "SDL2/SDL.h"

#include <stdexcept>

#include "Entity.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

#include "GraphicsComponent.hpp"

GraphicsComponent::GraphicsComponent() :
    run(AnimatedSprite("assets/images/playerrun.png", 624, 58, 78, 58, 16, 28, 23, 16, 10)),
    idle(AnimatedSprite("assets/images/playeridle.png", 858, 58, 78, 58, 16, 28, 23, 16, 10)),
    attack(AnimatedSprite("assets/images/playerattack.png", 234, 58, 78, 58, 16, 28, 23, 16, 10)),
    jump(AnimatedSprite("assets/images/playerjump.png", 78, 58, 78, 58, 16, 28, 23, 16, 10)),
    fall(AnimatedSprite("assets/images/playerfall.png", 78, 58, 78, 58, 16, 28, 23, 16, 10)),
    creation_time(static_cast<int>(SDL_GetTicks()))
{}

void GraphicsComponent::paint(Entity& entity) {
    auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(entity.x, entity.y);
    assert (!entity.states.empty());
    if (entity.states.top() == EntityState::ATTACK && entity.graphics->attack.animation_complete(creation_time)) {
        entity.states.pop();
    }
    assert (!entity.states.empty());
    switch (entity.states.top()) {
        case EntityState::ATTACK:
            attack.paint(screen_x, screen_y, creation_time, entity.facing_left);
            break;
        case EntityState::MOVING:
            if (entity.speed_y < 0.0f) {
                jump.paint(screen_x, screen_y, creation_time, entity.facing_left);
            } else if (entity.speed_y > 0.0f && !entity.on_ground) {
                fall.paint(screen_x, screen_y, creation_time, entity.facing_left);
            } else if (entity.speed_x != 0) {
                run.paint(screen_x, screen_y, creation_time, entity.facing_left);
            } else if (entity.speed_x == 0) {
                idle.paint(screen_x, screen_y, creation_time, entity.facing_left);
            } else {
                throw std::runtime_error("Unexpected state");
            }
            break;
        default:
            throw std::runtime_error("Unexpected state");
    }
}

void GraphicsComponent::reset_time() {
    creation_time = SDL_GetTicks();
}