#pragma once

#include <vector>

#include "Scene.hpp"
#include "Tiles.hpp"

class Entity;

enum class LevelEvent {
    ATTACKED
};

class Level : public Scene {
public:
    Tilemap level;
    Entity* player;
    std::vector<Entity*> pigs;

    Level();
    ~Level();

    void handle_event(InputEvent e) override;
    void tick() override;
    void paint() override;
    void send_event(LevelEvent event, Entity* entity);
};