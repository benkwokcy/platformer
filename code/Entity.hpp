#pragma once

#include "Event.hpp"

// The interface for all entities.
class Entity {
public:
    virtual ~Entity() = default;
    virtual void paint() = 0;
    virtual void handle_event(Event e) = 0;
    virtual void tick() = 0; // computes the next frame
};