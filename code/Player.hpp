#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Window.hpp"
#include "Sprite.hpp"

class Player : public Entity {
public:
    Player() :
        sprite(Sprite("sprites/playerrun.png", 78, 58, 8, 10))
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(Event e) override {}

    void tick() override {}

private:
    Sprite sprite;
};