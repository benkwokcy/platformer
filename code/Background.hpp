#pragma once

#include <SDL2/SDL.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Sprite.hpp"
#include "Tiles.hpp"

// The background of the level.
class Background : public Entity {
public:
    Background() :
        bounding_box({0, 0, 640, 480}),
        sprite(Sprite("sprites/background.png", 640, 480, bounding_box)),
        tilemap("level.tmx")
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    SDL_Rect bounding_box;
    Sprite sprite;
    Tilemap tilemap;
};