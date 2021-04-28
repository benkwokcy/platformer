#pragma once

#include <vector>

#include "Scene.hpp"
#include "Tiles.hpp"
#include "HealthBar.hpp"
#include "LevelEvent.hpp"
#include "Door.hpp"

class Entity;

class Level : public Scene {
public:
    Tilemap level;
    Entity* player;
    std::vector<Entity*> pigs;
    HealthBar healthbar;
    Door start_door;

    Level();
    ~Level();

    void handle_event(InputEvent e) override;
    void tick() override;
    void paint() override;
    void send_event(LevelEvent event, Entity& entity);
    void enter() override;
};