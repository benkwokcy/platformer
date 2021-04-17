#pragma once

#include "Sprite.hpp"

class Entity;

class GraphicsComponent {
public:
    AnimatedSprite run, idle, attack, jump, fall;

    GraphicsComponent();
    void paint(Entity& entity);
};