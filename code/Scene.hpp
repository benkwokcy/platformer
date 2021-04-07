#pragma once

#include "Event.hpp"

// The interface for all scenes.
class Scene {
public:
    virtual ~Scene() {}
    virtual void paint() = 0;
    virtual void handle_event(Event e) = 0;
    virtual void tick() = 0; // computes the next frame
};