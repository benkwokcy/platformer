#pragma once

#include "Input.hpp"

class Entity;
class Tilemap;

class InputComponent {
public:
    virtual ~InputComponent() {};
    virtual void handle_event(Entity& me, InputEvent e) = 0;
    virtual void tick(Entity& me, Tilemap& level) = 0;
};

class PlayerInputComponent : public InputComponent {
public:
    void handle_event(Entity& me, InputEvent e) override;
    void tick(Entity& me, Tilemap& level) override;
};

class PigInputComponent : public InputComponent {
public:
    void handle_event(Entity& me, InputEvent e) override;
    void tick(Entity& me, Tilemap& level) override;
};
