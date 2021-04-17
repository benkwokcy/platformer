#pragma once

#include <SDL2/SDL.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Sprite.hpp"

class Menu : public Entity {
public:
    Menu() :
        sprite(Sprite("assets/images/mainmenu.png", 640, 480))
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(InputEvent e) override {}
    void tick() override {}

private:
    Sprite sprite;
};