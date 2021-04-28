#pragma once

struct SDL_Rect;
class Character;

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
    Touching touching {}; // keeps track where we are touching an immovable object

    void tick(Character& me);
    void collide_immovable(Character& me, const SDL_Rect& other);
    void collide_movable(Character& me, Character& other);
    void knockback(Character& me, float source_x);
    void die(Character& me);

    inline bool on_ground() {
        return touching.bottom == true;
    }
};