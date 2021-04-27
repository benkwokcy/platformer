#pragma once

#include "Sprite.hpp"
#include "LevelEvent.hpp"

class Entity;
class Level;

class HealthBar {
public:
    HealthBar();

    void handle_event(LevelEvent event, Level* level, Entity* other);
    void paint(int health);

    Sprite bar;
    AnimatedSprite heartidle;
    AnimatedSprite hearthit1, hearthit2, hearthit3;
    int x, y;
    bool isOneHit, isTwoHit, isThreeHit;
};