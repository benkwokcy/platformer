#pragma once

class Player;

enum class CollisionType {
    OVERLAP_TOP, OVERLAP_BOTTOM, OVERLAP_LEFT, OVERLAP_RIGHT, 
    TOUCH_TOP, TOUCH_BOTTOM, TOUCH_LEFT, TOUCH_RIGHT,
    NONE
};

CollisionType rect_collide_rect(const SDL_Rect& a, const SDL_Rect& b);

class CollisionComponent {
public:
    void collide_map(Player& player, const SDL_Rect& other);
};