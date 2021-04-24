#include "SDL2/SDL.h"

#include <stdexcept>
#include <array>

#include "Entity.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"
#include "PhysicsComponent.hpp"

#include "GraphicsComponent.hpp"

GraphicsComponent::GraphicsComponent(AnimatedSprite&& run, AnimatedSprite&& idle, AnimatedSprite&& attack, AnimatedSprite&& jump, 
                                     AnimatedSprite&& fall, AnimatedSprite&& ground, AnimatedSprite&& dead, AnimatedSprite&& hit) :
    run(std::move(run)),
    idle(std::move(idle)),
    attack(std::move(attack)),
    jump(std::move(jump)),
    fall(std::move(fall)),
    ground(std::move(ground)),
    dead(std::move(dead)),
    hit(std::move(hit))
{}

GraphicsComponent::GraphicsComponent(GraphicsComponent&& other) :
    run(std::move(other.run)),
    idle(std::move(other.idle)),
    attack(std::move(other.attack)),
    jump(std::move(other.jump)),
    fall(std::move(other.fall)),
    ground(std::move(other.ground)),
    dead(std::move(other.dead)),
    hit(std::move(other.hit))
{}

AnimatedSprite& GraphicsComponent::current_sprite(Entity& me) {
    switch(me.current_state()) {
        case EntityState::ATTACK:
            return attack;
            break;
        case EntityState::GROUND:
            return ground;
            break;
        case EntityState::DEAD:
            return dead;
            break;
        case EntityState::HIT:
            return hit;
            break;
        case EntityState::MOVING:
            if (me.speed_y < 0.0f) {
                return jump;
            } else if (me.speed_y > 0.0f && !me.physics->on_ground()) {
                return fall;
            } else if (me.speed_x != 0) {
                return run;
            } else if (me.speed_x == 0) {
                return idle;
            } else {
                throw std::runtime_error("Unexpected state");
            }
            break;
        default:
            throw std::runtime_error("Unexpected state");
    }
}

void GraphicsComponent::paint(Entity& me) {
    while (me.states.size() > 1 && me.current_state() != EntityState::DEAD && current_sprite(me).loops_completed()) {
        me.states.pop();
    }
    auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(me.x, me.y);
    current_sprite(me).paint(screen_x, screen_y, me.facing_left);
}