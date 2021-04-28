#pragma once

#include "Input.hpp"

// The interface for all entities.
class Scene {
public:
    virtual ~Scene() = default;
    virtual void paint() = 0;
    virtual void handle_event(InputEvent e) = 0;
    virtual void tick() = 0; // computes the next frame
    virtual void enter() = 0; // triggers any special events that should happen when we switch to this scene
};