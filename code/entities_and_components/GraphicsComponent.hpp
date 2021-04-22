#pragma once

#include "Sprite.hpp"

class Entity;

class GraphicsComponent {
public:
    AnimatedSprite run, idle, attack, jump, fall, ground, dead;

    GraphicsComponent(AnimatedSprite&& run, AnimatedSprite&& idle, AnimatedSprite&& attack, AnimatedSprite&& jump, 
                      AnimatedSprite&& fall, AnimatedSprite&& ground, AnimatedSprite&& dead);
                      
    void paint(Entity& me);
};