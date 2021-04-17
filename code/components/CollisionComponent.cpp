#include "SDL2/SDL.h"

#include <algorithm>

#include "Player.hpp"

#include "CollisionComponent.hpp"

/*********************************************
 *           COLLISION ALGORITHMS
 *********************************************/

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

/*********************************************
 *              COMPONENTS
 *********************************************/

void CollisionComponent::collide_map(Player& player, const SDL_Rect& other) {
    SDL_Rect me = player.bounding_box();
    switch (rect_collide_rect(me, other)) {
        case CollisionType::OVERLAP_TOP:
            player.y += (other.y + other.h) - player.y;
            player.speed_y = std::max(player.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_BOTTOM:
            player.y -= (player.y + player.h) - other.y;
            player.on_ground = true;
            player.speed_y = std::min(player.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_LEFT:
            player.x += (other.x + other.w) - player.x;
            player.speed_x = 0.0f;
            break;
        case CollisionType::OVERLAP_RIGHT:
            player.x -= (player.x + player.w) - other.x;
            player.speed_x = 0.0f;
            break;
        case CollisionType::TOUCH_BOTTOM:
            player.on_ground = true;
            break;
        default:
            break;
    }
}