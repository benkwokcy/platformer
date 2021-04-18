#pragma once

#include "Sprite.hpp"

class Entity;

class GraphicsComponent {
public:
    AnimatedSprite run, idle, attack, jump, fall;
    int creation_time;

    GraphicsComponent(AnimatedSprite&& run, AnimatedSprite&& idle, AnimatedSprite&& attack, AnimatedSprite&& jump, AnimatedSprite&& fall);
    void paint(Entity& entity);
    void reset_time();
};