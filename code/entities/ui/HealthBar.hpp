#pragma once

#include "Sprite.hpp"
#include "LevelEvent.hpp"

class Character;
class Level;

class HealthBar {
public:
    HealthBar();

    void handle_event(LevelEvent event, Level& level, Character& other);
    void paint(int health);

    Sprite& bar;
    AnimatedSprite heartidle;
    AnimatedSprite hearthit1, hearthit2, hearthit3;
    int x, y;
    bool isOneHit, isTwoHit, isThreeHit;
};