#pragma once

#include <memory>
#include <vector>

#include "Entity.hpp"
#include "Player.hpp"
#include "Background.hpp"

class Level : public Entity {
public:
    Level() {
        entities.emplace_back(make_unique<Background>());
        entities.emplace_back(make_unique<Player>());
    }

    void paint() override {
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
    std::vector<unique_ptr<Entity>> entities;
};