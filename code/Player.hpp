#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Entity.hpp"
#include "Window.hpp"
#include "Sprite.hpp"
#include "Event.hpp"

enum class PlayerState {
    IDLE, RUN_LEFT, RUN_RIGHT
};

class Player : public Entity {
public:
    Player() :
        run(Sprite("sprites/playerrun.png", 78, 58, 8, 10)),
        idle(Sprite("sprites/playeridle.png", 78, 58, 11, 10)),
        state(PlayerState::IDLE),
        x(Window::center_x()),
        y(Window::center_y()),
        speed_x(0.0f),
        speed_y(0.0f),
        facing_left(false)
    {
        sprite = &idle;
    }

    void paint() override {
        sprite->paint(x, y, facing_left);
    }

    void handle_event(Event e) override {
        switch (e) {
            case Event::LEFT_PRESS:
                if (state != PlayerState::RUN_LEFT) {
                    state = PlayerState::RUN_LEFT;
                    facing_left = true;
                    sprite = &run;
                    sprite->set_first_frame();
                    speed_x = -0.5f;
                }
                break;
            case Event::RIGHT_PRESS:
                if (state != PlayerState::RUN_RIGHT) {
                    state = PlayerState::RUN_RIGHT;
                    facing_left = false;
                    sprite = &run;
                    sprite->set_first_frame();
                    speed_x = 0.5f;
                }
                break;
            case Event::LEFT_RELEASE:
                if (speed_x < 0) {
                    state = PlayerState::IDLE;
                    sprite = &idle;
                    sprite->set_first_frame();
                    speed_x = 0.0f;
                }
                break;
            case Event::RIGHT_RELEASE:
                if (speed_x > 0) {
                    state = PlayerState::IDLE;
                    sprite = &idle;
                    sprite->set_first_frame();
                    speed_x = 0.0f;
                }
                break;
            default:
                break;
        }
    }

    void tick() override {
        x += speed_x;
        y += speed_y;
    }

private:
    // sprites
    Sprite run;
    Sprite idle;
    // player info
    PlayerState state;
    Sprite* sprite;
    float x;
    float y;
    float speed_x;
    float speed_y;
    bool facing_left;
};