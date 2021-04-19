#pragma once

#include <utility>

struct SDL_Rect;
class Entity;

// TODO - only need 5 types? top/bottom/left/right/none. The penetration tells whether its touching or overlapping.
// Might be good to keep them separate though if it's cleaner.
enum class CollisionType {
    OVERLAP_TOP, OVERLAP_BOTTOM, OVERLAP_LEFT, OVERLAP_RIGHT, 
    TOUCH_TOP, TOUCH_BOTTOM, TOUCH_LEFT, TOUCH_RIGHT,
    NONE
};

std::pair<CollisionType,int> rect_collide_rect(const SDL_Rect& a, const SDL_Rect& b);

struct Touching {
    bool top = false;
    bool bottom = false;
    bool left = false;
    bool right = false;
};

class PhysicsComponent {
public:
    Touching touching {}; // keeps track where we are touching an immoveable object

    void tick(Entity& me);
    void collide_immovable(Entity& me, const SDL_Rect& other);
    void collide_movable(Entity& me, Entity& other);

    inline bool on_ground() {
        return touching.bottom == true;
    }
};