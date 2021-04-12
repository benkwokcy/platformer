#include <SDL2/SDL.h>

#include <algorithm>

enum class CollisionType {
    TOP, BOTTOM, LEFT, RIGHT, NONE
};

// From a's perspective. Call this if you already know there was a collision.
CollisionType rect_collide_rect(const SDL_Rect& a, const SDL_Rect& b) {
    int top_overlap = abs((b.y + b.h) - a.y);
    int bottom_overlap = abs((a.y + a.h) - b.y);
    int left_overlap = abs((b.x + b.w) - a.x);
    int right_overlap = abs((a.x + a.w) - b.x);

    int mini = std::min({top_overlap, bottom_overlap, left_overlap, right_overlap});

    if (mini == top_overlap) {
        return CollisionType::TOP;
    } else if (mini == bottom_overlap) {
        return CollisionType::BOTTOM;
    } else if (mini == left_overlap) {
        return CollisionType::LEFT;
    } else if (mini == right_overlap) {
        return CollisionType::RIGHT;
    } else {
        return CollisionType::NONE;
    }
}