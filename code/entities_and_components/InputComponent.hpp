#pragma once

#include "Input.hpp"

class Entity;

class InputComponent {
public:
    virtual ~InputComponent() {};
    virtual void handle_event(Entity& me, InputEvent e) = 0;
    virtual void tick(Entity& me) = 0;
};

class PlayerInputComponent : public InputComponent {
public:
    virtual void handle_event(Entity& me, InputEvent e);
    virtual void tick(Entity& me);
};

class PigInputComponent : public InputComponent {
public:
    virtual void handle_event(Entity& me, InputEvent e);
    virtual void tick(Entity& me);
};
