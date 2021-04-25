#pragma once

#include "SDL2/SDL.h"

#include "Sprite.hpp"
#include "Window.hpp"

class HealthBar {
public:
    HealthBar() :
        bar(Sprite("assets/images/playerhealthbar.png", 66, 34)),
        x(Window::width - 66 - 15),
        y(15)
    {}

    void paint() {
        bar.paint(x, y);
    }

    Sprite bar;
    int x, y;
};