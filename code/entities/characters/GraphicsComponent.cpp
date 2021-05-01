#include "SDL2/SDL.h"

#include <stdexcept>
#include <array>

#include "Character.hpp"
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

AnimatedSprite& GraphicsComponent::get_current_sprite(Character& me) {
    switch(me.current_state()) {
        case CharacterState::ATTACK:
            return attack;
            break;
        case CharacterState::GROUND:
            return ground;
            break;
        case CharacterState::DEAD:
            return dead;
            break;
        case CharacterState::HIT:
            return hit;
            break;
        case CharacterState::MOVING:
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

void GraphicsComponent::paint(Character& me) {
    while (me.states.size() > 1 && me.current_state() != CharacterState::DEAD && get_current_sprite(me).loops_completed()) {
        me.states.pop();
    }
    if (!Camera::is_box_visible(me.x, me.y, me.w, me.h)) { return; }
    auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(me.x, me.y);
    get_current_sprite(me).paint(screen_x, screen_y, me.facing_left);
}