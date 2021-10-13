#pragma once

#include <stdexcept>

#include "Sprite.hpp"
#include "Assets.hpp"
#include "Camera.hpp"

enum class DoorState {
    IDLE, OPENING
};

class Door {
public:
    Door(int x, int y) :
        x(x),
        y(y),
        w(46.0f),
        h(56.0f),
        current_state(DoorState::IDLE),
        idle(AnimatedSprite(Assets::instance().door_idle)),
        opening(AnimatedSprite(Assets::instance().door_opening))
    {}

    void paint() {
        auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(x, y);
        if (current_state != DoorState::IDLE && get_current_sprite().loops_completed()) {
            current_state = DoorState::IDLE;
        }
        if (!Camera::is_box_visible(x, y, w, h)) { return; }
        get_current_sprite().paint(screen_x, screen_y);
    }

    void open() {
        current_state = DoorState::OPENING;
        opening.reset_time();
    }

    float x, y;
    float w, h;
    DoorState current_state;
    AnimatedSprite idle, opening;
private:
    AnimatedSprite& get_current_sprite() {
        switch (current_state) {
            case DoorState::IDLE:
                return idle;
            case DoorState::OPENING:
                return opening;
        }
    }
};