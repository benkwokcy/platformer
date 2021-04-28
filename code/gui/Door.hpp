#pragma once

#include "Sprite.hpp"
#include "Assets.hpp"
#include "Camera.hpp"

class Door {
public:
    Door(int x, int y) :
        x(x),
        y(y),
        idle(Assets::PATH + "images/dooridle.png", 46, 56, 46, 56, 46, 56, 0, 0, 10)
    {}

    void paint() {
        auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(x, y);
        idle.paint(screen_x, screen_y);
    }

    int x, y;
    AnimatedSprite idle;
};