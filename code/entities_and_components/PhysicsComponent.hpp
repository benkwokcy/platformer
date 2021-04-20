#pragma once

#include <utility>

struct SDL_Rect;
class Entity;

namespace CollisionDetection {
    bool is_overlapping(SDL_Rect& a, SDL_Rect& b);
};

enum class CollisionType {
    OVERLAP_TOP, OVERLAP_BOTTOM, OVERLAP_LEFT, OVERLAP_RIGHT, 
    TOUCH_TOP, TOUCH_BOTTOM, TOUCH_LEFT, TOUCH_RIGHT, // touching means the distance between the two is 0.
    NONE
};

// Touching means I am touching an immovable object on that side or I'm touching a movable object that is touching an immovable object.
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