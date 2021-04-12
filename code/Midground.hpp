#pragma once

#include <SDL2/SDL.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Sprite.hpp"
#include "Tiles.hpp"

class Midground : public Entity {
public:
    Midground() :
        tilemap("level.tmx")
    {}

    void paint() override {
        auto& M = tilemap.layers[0];

        for (int r = 0; r < M.size(); r++) {
            for (int c = 0; c < M[0].size(); c++) {
                if (M[r][c] != 0) {
                    tilemap.paint(c * tilemap.tile_width, r * tilemap.tile_height, M[r][c]);
                }
            }
        }
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    Tilemap tilemap;
};