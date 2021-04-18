#pragma once

#include "Sprite.hpp"

class Entity;

class GraphicsComponent {
public:
    AnimatedSprite run, idle, attack, jump, fall, ground;

    GraphicsComponent(AnimatedSprite&& run, AnimatedSprite&& idle, AnimatedSprite&& attack, AnimatedSprite&& jump, AnimatedSprite&& fall, AnimatedSprite&& ground);
    void paint(Entity& entity);
};