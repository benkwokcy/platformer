#include "SDL2/SDL.h"

#include <stdexcept>

#include "Entity.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

#include "GraphicsComponent.hpp"

GraphicsComponent::GraphicsComponent(AnimatedSprite&& run, AnimatedSprite&& idle, AnimatedSprite&& attack, AnimatedSprite&& jump, AnimatedSprite&& fall) :
    run(std::move(run)),
    idle(std::move(idle)),
    attack(std::move(attack)),
    jump(std::move(jump)),
    fall(std::move(fall)),
    creation_time(static_cast<int>(SDL_GetTicks()))
{}

void GraphicsComponent::paint(Entity& entity) {
    assert (!entity.states.empty());
    if (entity.states.top() == EntityState::ATTACK && entity.graphics->attack.animation_complete(creation_time)) {
        entity.states.pop();
        assert (!entity.states.empty());
    }
    auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(entity.x, entity.y);
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