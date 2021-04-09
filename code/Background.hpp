#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Window.hpp"
#include "Sprite.hpp"

// The main menu.
class Background : public Entity {
public:
    Background() :
        sprite(Sprite("sprites/background.png", 640, 480))
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    Sprite sprite;
};