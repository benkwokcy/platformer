#pragma once

#include <SDL2/SDL.h>

#include <iostream>
#include <stdexcept>
#include <stack>

#include "Entity.hpp"
#include "Window.hpp"
#include "Sprite.hpp"
#include "Input.hpp"
#include "Collision.hpp"
#include "Physics.hpp"
#include "Camera.hpp"

enum class PigState {
    MOVING, ATTACK
};

class Pig : public Entity {
public:
    Pig(float x, float y) :
        x(x),
        y(y),
        w(15.0f),
        h(16.0f),
        speed_x(0.0f),
        speed_y(0.0f),
        facing_left(false),
        on_ground(false),
        run(AnimatedSprite("sprites/pigidle.png", 374, 28, 34, 28, w, h, 12, 12, 10)),
        idle(AnimatedSprite("sprites/pigidle.png", 374, 28, 34, 28, w, h, 12, 12, 10)),
        attack(AnimatedSprite("sprites/pigidle.png", 374, 28, 34, 28, w, h, 12, 12, 10)),
        jump(AnimatedSprite("sprites/pigidle.png", 374, 28, 34, 28, w, h, 12, 12, 10)),
        fall(AnimatedSprite("sprites/pigidle.png", 374, 28, 34, 28, w, h, 12, 12, 10))
    {
        states.push(PigState::MOVING);
    }

    void paint() override {
        auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(x, y);
        assert (!states.empty());
        switch (states.top()) {
            case PigState::ATTACK:
                attack.paint(screen_x, screen_y, facing_left);
                break;
            case PigState::MOVING:
                if (speed_y < 0.0f) {
                    jump.paint(screen_x, screen_y, facing_left);
                } else if (speed_y > 0.0f && !on_ground) {
                    fall.paint(screen_x, screen_y, facing_left);
                } else if (speed_x != 0) {
                    run.paint(screen_x, screen_y, facing_left);
                } else if (speed_x == 0) {
                    idle.paint(screen_x, screen_y, facing_left);
                } else {
                    throw std::runtime_error("Unexpected state");
                }
                break;
            default:
                throw std::runtime_error("Unexpected state");
        }
    }

    void handle_event(InputEvent e) override {
        // switch (e) {
        //     case InputEvent::LEFT:
        //         facing_left = true;
        //         break;
        //     case InputEvent::RIGHT:
        //         facing_left = false;
        //         break;
        //     case InputEvent::ATTACK:
        //         if (states.top() != PigState::ATTACK) {
        //             states.push(PigState::ATTACK);
        //             attack.set_first_frame();
        //         }
        //         break;
        //     case InputEvent::JUMP:
        //         if (on_ground) {
        //             speed_y -= 11.0f;
        //         }
        //     default:
        //         break;
        // }
    }

    void tick() override {
        // if (Input::left_down() || Input::right_down()) {
        //     if (facing_left) {
        //         speed_x = -4.0f;
        //     } else {
        //         speed_x = 4.0f;
        //     }
        // } else {
        //     speed_x = 0.0f;
        // }
        on_ground = false; // assume we are not on the ground and let the collision logic tell us if we are.
        speed_y += Physics::GRAVITY;
        x += speed_x;
        y += speed_y;
        if (states.top() == PigState::ATTACK && attack.animation_complete()) {
            states.pop();
        }
    }

    void collide_map(const SDL_Rect& other) {
        SDL_Rect me = bounding_box();
        switch (rect_collide_rect(me, other)) {
            case CollisionType::OVERLAP_TOP:
                y += (other.y + other.h) - y;
                speed_y = std::max(speed_y, 0.0f);
                break;
            case CollisionType::OVERLAP_BOTTOM:
                y -= (y + h) - other.y;
                on_ground = true;
                speed_y = std::min(speed_y, 0.0f);
                break;
            case CollisionType::OVERLAP_LEFT:
                x += (other.x + other.w) - x;
                speed_x = 0.0f;
                break;
            case CollisionType::OVERLAP_RIGHT:
                x -= (x + w) - other.x;
                speed_x = 0.0f;
                break;
            case CollisionType::TOUCH_BOTTOM:
                on_ground = true;
                break;
            default:
                break;
        }
    }

    float get_x() {
        return x;
    }

private:
    float x, y; // the top left corner of the player in level coordinates
    float w, h; // dimensions of the bounding box
    float speed_x, speed_y;
    bool facing_left, on_ground;
    std::stack<PigState> states;
    AnimatedSprite run, idle, attack, jump, fall;

    SDL_Rect bounding_box() {
        return { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
    }
};