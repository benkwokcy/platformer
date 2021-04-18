#pragma once

#include <memory>
#include <vector>

#include "Scene.hpp"
#include "Tiles.hpp"
#include "Camera.hpp"
#include "Entity.hpp"

class Level : public Scene {
public:
    Level() :
        tilemap("assets/level.tmx")
    {
        player = create_player(tilemap.markers.at("PlayerSpawn").x, tilemap.markers.at("PlayerSpawn").y);
        pig = create_player(tilemap.markers.at("PigSpawn").x, tilemap.markers.at("PigSpawn").y);
    }

    ~Level() {
        delete player;
        delete pig;
    }

    void paint() override {
        tilemap.paint();
        player->paint();
        pig->paint();
    }

    void handle_event(InputEvent e) override {
        player->handle_event(e);
        pig->handle_event(e);
    }

    void tick() override {
        player->tick();
        pig->tick();
        for (auto& c : tilemap.collisions) {
            player->collide_map(c);
            pig->collide_map(c);
        }
        // TODO - pig collide player
        Camera::tick(static_cast<int>(player->x));
    }

private:
    Tilemap tilemap;
    Entity* player;
    Entity* pig;
};