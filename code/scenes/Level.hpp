#pragma once

#include <memory>
#include <algorithm>
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
        pigs.push_back(create_pig(level.markers.at("PigSpawn").x, level.markers.at("PigSpawn").y));
    }

    ~Level() {
        delete player;
        for (auto& pig : pigs) { 
            delete pig; 
        }
    }

    void paint() override {
        level.paint();
        player->paint();
        for (auto& pig : pigs) { 
            pig->paint(); 
        }
    }

    void handle_event(InputEvent e) override {
        player->handle_event(e);
        for (auto& pig : pigs) { 
            pig->handle_event(e); 
        }
    }

    void tick() override {
        // update position
        player->tick();
        for (auto& pig : pigs) { 
            pig->tick(); 
        }
        // collide with level
        for (auto& c : level.collisions) {
            player->collide_immovable(c);
            for (auto& pig : pigs) { 
                pig->collide_immovable(c); 
            }
        }
        // collide with objects
        for (auto& pig : pigs) { 
            player->collide_movable(*pig); 
        }
        // update camera
        Camera::tick(static_cast<int>(player->x));
    }

private:
    Tilemap level;
    Entity* player;
    std::vector<Entity*> pigs;
};