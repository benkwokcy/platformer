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
        sprite(Sprite("sprites/background.png", 640, 480, 0, 0)),
        tilemap("level.tmx")
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    Sprite sprite;
    Tilemap tilemap;
};