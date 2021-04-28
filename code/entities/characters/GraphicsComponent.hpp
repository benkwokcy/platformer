#pragma once

#include "Sprite.hpp"

class Character;

class GraphicsComponent {
public:
    AnimatedSprite run, idle, attack, jump, fall, ground, dead, hit;

    GraphicsComponent(AnimatedSprite&& run, AnimatedSprite&& idle, AnimatedSprite&& attack, AnimatedSprite&& jump, 
                      AnimatedSprite&& fall, AnimatedSprite&& ground, AnimatedSprite&& dead, AnimatedSprite&& hit);

    GraphicsComponent(const GraphicsComponent& other) = delete;
    GraphicsComponent& operator=(const GraphicsComponent& other) = delete;
    GraphicsComponent(GraphicsComponent&& other);
    GraphicsComponent& operator=(GraphicsComponent&& other) = delete;
                      
    void paint(Character& me);
    AnimatedSprite& get_current_sprite(Character& me);
};