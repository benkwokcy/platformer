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
        level("assets/level.tmx")
    {
        player = create_player(level.markers.at("PlayerSpawn").x, level.markers.at("PlayerSpawn").y);
        pig = create_pig(level.markers.at("PigSpawn").x, level.markers.at("PigSpawn").y);
    }

    ~Level() {
        delete player;
        delete pig;
    }

    void paint() override {
        level.paint();
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
        for (auto& c : level.collisions) {
            player->collide_immovable(c);
            pig->collide_immovable(c);
        }
        player->collide_movable(pig->bounding_box());
        Camera::tick(static_cast<int>(player->x));
    }

private:
    Tilemap level;
    Entity* player;
    Entity* pig;
};