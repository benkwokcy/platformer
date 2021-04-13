#include <SDL2/SDL.h>

#include <algorithm>

enum class CollisionType {
    OVERLAP_TOP, OVERLAP_BOTTOM, OVERLAP_LEFT, OVERLAP_RIGHT, 
    TOUCH_TOP, TOUCH_BOTTOM, TOUCH_LEFT, TOUCH_RIGHT,
    NONE
};

// From a's perspective.
CollisionType rect_collide_rect(const SDL_Rect& a, const SDL_Rect& b) {
    int top_overlap = abs((b.y + b.h) - a.y);
    int bottom_overlap = abs((a.y + a.h) - b.y);
    int left_overlap = abs((b.x + b.w) - a.x);
    int right_overlap = abs((a.x + a.w) - b.x);

    int mini = std::min({top_overlap, bottom_overlap, left_overlap, right_overlap});

    if (SDL_HasIntersection(&a, &b) == SDL_TRUE) {
        if (mini == top_overlap) {
            return CollisionType::OVERLAP_TOP;
        } else if (mini == bottom_overlap) {
            return CollisionType::OVERLAP_BOTTOM;
        } else if (mini == left_overlap) {
            return CollisionType::OVERLAP_LEFT;
        } else if (mini == right_overlap) {
            return CollisionType::OVERLAP_RIGHT;
        }
    } else if (mini == 0) {
        if (mini == top_overlap) {
            return CollisionType::TOUCH_TOP;
        } else if (mini == bottom_overlap) {
            return CollisionType::TOUCH_BOTTOM;
        } else if (mini == left_overlap) {
            return CollisionType::TOUCH_LEFT;
        } else if (mini == right_overlap) {
            return CollisionType::TOUCH_RIGHT;
        }
    }

    return CollisionType::NONE;
}