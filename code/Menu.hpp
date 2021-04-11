#pragma once

#include <SDL2/SDL.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Sprite.hpp"

class Menu : public Entity {
public:
    Menu() :
        sprite(Sprite("sprites/mainmenu.png", 640, 480, 0, 0))
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    Sprite sprite;
};