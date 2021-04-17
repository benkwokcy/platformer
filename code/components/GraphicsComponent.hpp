#pragma once

#include "Sprite.hpp"

class Player;

class GraphicsComponent {
public:
    AnimatedSprite run, idle, attack, jump, fall;

    GraphicsComponent();
    void paint(Player& player);
};