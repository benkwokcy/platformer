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
    {
        entities.emplace_back(std::make_unique<Player>());
    }

    void paint() override {
        tilemap.paint();
        for (auto& e : entities) {
            e->paint();
        }
    }

    void handle_event(Event event) override {
        for (auto& e : entities) {
            e->handle_event(event);
        }
    }

    void tick() override {
        for (auto& e : entities) {
            e->tick();
        }
    }

private:
    Tilemap tilemap;
    std::vector<std::unique_ptr<Entity>> entities;
};