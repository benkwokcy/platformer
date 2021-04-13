#pragma once

#include "Input.hpp"

// The interface for all entities.
class Entity {
public:
    virtual ~Entity() = default;
    virtual void paint() = 0;
    virtual void handle_event(InputEvent e) = 0;
    virtual void tick() = 0; // computes the next frame
};