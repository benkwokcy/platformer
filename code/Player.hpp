#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
        state(PlayerState::IDLE),
        x(Window::center_x()),
        y(Window::center_y()),
        speed_x(0),
        speed_y(0)
    {
        sprite = &run;
    }

    void paint() override {
        sprite->paint(x, y);
    }

    void handle_event(Event e) override {
        switch (e) {
            case Event::LEFT_PRESS:
                state = PlayerState::RUN_LEFT;
                // change sprite and reset frame
                speed_x = -1;
                break;
            case Event::RIGHT_PRESS:
                state = PlayerState::RUN_RIGHT;
                // change sprite and reset frame
                speed_x = 1;
                break;
            case Event::LEFT_RELEASE:
                if (speed_x < 0) {
                    state = PlayerState::IDLE;
                    // change sprite and reset frame
                    speed_x = 0;
                }
                break;
            case Event::RIGHT_RELEASE:
                if (speed_x > 0) {
                    state = PlayerState::IDLE;
                    // change sprite and reset frame
                    speed_x = 0;
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
    Sprite run;

    PlayerState state;
    Sprite* sprite;
    int x;
    int y;
    int speed_x;
    int speed_y;
};