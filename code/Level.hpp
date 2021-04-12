#pragma once

#include <memory>
#include <vector>

#include "Entity.hpp"
#include "Player.hpp"
#include "Tiles.hpp"

class Level : public Entity {
public:
    Level() :
        tilemap("level.tmx")
    {}

    void paint() override {
        tilemap.paint();
        player.paint();
    }

    void handle_event(Event event) override {
        player.handle_event(event);
    }

    void tick() override {
        player.tick();
        for (auto& c : tilemap.collisions) {
            player.collide_map(c);
        }
    }

private:
    Player player;
    Tilemap tilemap;
};