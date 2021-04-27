#pragma once

#include <vector>

#include "Scene.hpp"
#include "Tiles.hpp"
#include "HealthBar.hpp"
#include "LevelEvent.hpp"

class Entity;

class Level : public Scene {
public:
    Tilemap level;
    Entity* player;
    std::vector<Entity*> pigs;
    HealthBar healthbar;

    Level();
    ~Level();

    void handle_event(InputEvent e) override;
    void tick() override;
    void paint() override;
    void send_event(LevelEvent event, Entity& entity);
};