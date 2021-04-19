#pragma once

class Entity;

enum class CollisionType {
    OVERLAP_TOP, OVERLAP_BOTTOM, OVERLAP_LEFT, OVERLAP_RIGHT, 
    TOUCH_TOP, TOUCH_BOTTOM, TOUCH_LEFT, TOUCH_RIGHT,
    NONE
};

struct Touching {
    bool top = false;
    bool bottom = false;
    bool left = false;
    bool right = false;
};

CollisionType rect_collide_rect(const SDL_Rect& a, const SDL_Rect& b);

class CollisionComponent {
public:
    Touching touching {}; // keeps track where we are touching an immoveable object

    bool on_ground();
    void reset_touching();
    void collide_movable(Entity& entity, Entity& other);
    void collide_immovable(Entity& entity, const SDL_Rect& other);
};