#pragma once

#include "Input.hpp"

class Entity;

class InputComponent {
public:
    void handle_event(Entity& me, InputEvent e);
    void tick(Entity& me);
};
