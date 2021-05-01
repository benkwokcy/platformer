#pragma once

#include <vector>

#include "Scene.hpp"
#include "Tiles.hpp"
#include "HealthBar.hpp"
#include "LevelEvent.hpp"
#include "Door.hpp"

class Character;

class Level : public Scene {
public:
    Tilemap level;
    Character* player;
    std::vector<Character*> pigs;
    HealthBar healthbar;
    Door start_door;
    Door end_door;

    Level();
    ~Level();

    void handle_event(InputEvent e) override;
    void tick() override;
    void paint() override;
    void send_event(LevelEvent event, Character& character);
    void enter() override;
};