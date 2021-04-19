#include "SDL2/SDL.h"

#include <algorithm>
#include <iostream>

#include "Entity.hpp"
#include "GraphicsComponent.hpp"

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

void CollisionComponent::collide(Entity& entity, const SDL_Rect& other) {
    SDL_Rect me = entity.bounding_box();
    switch (rect_collide_rect(me, other)) {
        case CollisionType::OVERLAP_TOP:
            entity.y += (other.y + other.h) - entity.y;
            entity.speed_y = std::max(entity.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_BOTTOM:
            entity.y -= (entity.y + entity.h) - other.y;
            entity.on_ground = true;
            if (entity.speed_y > 2.0f && entity.states.top() != EntityState::ATTACK) { 
                entity.states.push(EntityState::GROUND);
                entity.graphics->ground.reset_time();
            }
            entity.speed_y = std::min(entity.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_LEFT:
            entity.x += (other.x + other.w) - entity.x;
            entity.speed_x = 0.0f;
            break;
        case CollisionType::OVERLAP_RIGHT:
            entity.x -= (entity.x + entity.w) - other.x;
            entity.speed_x = 0.0f;
            break;
        case CollisionType::TOUCH_BOTTOM:
            entity.on_ground = true;
            break;
        default:
            break;
    }
}