#pragma once

#include "Sprite.hpp"

class Entity;

class GraphicsComponent {
public:
    AnimatedSprite run, idle, attack, jump, fall;
    int creation_time;

    GraphicsComponent();
    void paint(Entity& entity);
    void reset_time();
};