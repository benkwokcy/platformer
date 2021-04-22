#include "SDL2/SDL.h"

#include <stdexcept>

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

void GraphicsComponent::paint(Entity& me) {
    while ((me.current_state() == EntityState::ATTACK && me.graphics->attack.loops_completed()) || 
           (me.current_state() == EntityState::GROUND && me.graphics->ground.loops_completed()) ||
           (me.current_state() == EntityState::HIT && me.graphics->hit.loops_completed())) {
        me.states.pop();
    }
    auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(me.x, me.y);
    switch (me.current_state()) {
        case EntityState::ATTACK:
            attack.paint(screen_x, screen_y, me.facing_left);
            break;
        case EntityState::GROUND:
            ground.paint(screen_x, screen_y, me.facing_left);
            break;
        case EntityState::DEAD:
            dead.paint(screen_x, screen_y, me.facing_left);
            break;
        case EntityState::HIT:
            hit.paint(screen_x, screen_y, me.facing_left);
            break;
        case EntityState::MOVING:
            if (me.speed_y < 0.0f) {
                jump.paint(screen_x, screen_y, me.facing_left);
            } else if (me.speed_y > 0.0f && !me.physics->on_ground()) {
                fall.paint(screen_x, screen_y, me.facing_left);
            } else if (me.speed_x != 0) {
                run.paint(screen_x, screen_y, me.facing_left);
            } else if (me.speed_x == 0) {
                idle.paint(screen_x, screen_y, me.facing_left);
            } else {
                throw std::runtime_error("Unexpected state");
            }
            break;
        default:
            throw std::runtime_error("Unexpected state");
    }
}