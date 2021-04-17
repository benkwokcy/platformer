#pragma once

#include "Input.hpp"

class Entity;

class InputComponent {
public:
    void handle_event(Entity& entity, InputEvent e);
    void tick(Entity& entity);
};
