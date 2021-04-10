#pragma once

#include <SDL2/SDL.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Sprite.hpp"

// The main menu.
class Menu : public Entity {
public:
    Menu() :
        bounding_box({0, 0, 640, 480}),
        sprite(Sprite("sprites/mainmenu.png", 640, 480, bounding_box))
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    SDL_Rect bounding_box;
    Sprite sprite;
};