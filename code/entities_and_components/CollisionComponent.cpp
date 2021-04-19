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
// TODO - return the penetration amount
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

void CollisionComponent::reset_touching() {
    touching = { false, false, false, false };
}

bool CollisionComponent::on_ground() {
    return touching.bottom == true;
}

void CollisionComponent::collide_immovable(Entity& me, const SDL_Rect& other) {
    auto me_box = me.bounding_box();
    switch (rect_collide_rect(me_box, other)) {
        case CollisionType::OVERLAP_TOP:
            touching.top = true;
            me.y += (other.y + other.h) - me.y;
            me.speed_y = std::max(me.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_BOTTOM:
            touching.bottom = true;
            me.y -= (me_box.y + me_box.h) - other.y;
            if (me.speed_y > 2.0f && me.states.top() != EntityState::ATTACK) { 
                me.states.push(EntityState::GROUND);
                me.graphics->ground.reset_time();
            }
            me.speed_y = std::min(me.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_LEFT:
            touching.left = true;
            me.x += (other.x + other.w) - me_box.x;
            me.speed_x = 0.0f;
            break;
        case CollisionType::OVERLAP_RIGHT:
            touching.right = true;
            me.x -= (me_box.x + me_box.w) - other.x;
            me.speed_x = 0.0f;
            break;
        case CollisionType::TOUCH_TOP:
            touching.top = true;
            break;
        case CollisionType::TOUCH_BOTTOM:
            touching.bottom = true;
            break;
        case CollisionType::TOUCH_LEFT:
            touching.left = true;
            break;
        case CollisionType::TOUCH_RIGHT:
            touching.right = true;
            break;
        default:
            break;
    }
}

// Same direction vs opposite direction
// Move both half instead of moving one all the way.
void CollisionComponent::collide_movable(Entity& me, Entity& other) {
    auto me_box = me.bounding_box();
    auto other_box = other.bounding_box();
    switch (rect_collide_rect(me_box, other_box)) {
        case CollisionType::OVERLAP_TOP:
            if (touching.bottom) {
                other.y -= (other_box.y + other_box.h) - me.y;
                other.speed_y = std::max(other.speed_y, 0.0f);
            } else {
                me.y += (other_box.y + other_box.h) - me.y;
                me.speed_y = std::max(me.speed_y, 0.0f);
            }
            break;
        case CollisionType::OVERLAP_BOTTOM:
            if (other.collision->on_ground()) {
                me.y -= (me.y + me.h) - other_box.y;
                me.speed_y = std::min(me.speed_y, 0.0f);
                touching.bottom = true;
                if (me.speed_y > 2.0f && me.states.top() != EntityState::ATTACK) { 
                    me.states.push(EntityState::GROUND);
                    me.graphics->ground.reset_time();
                }
            } else {
                other.y += (me.y + me.h) - other_box.y;
                other.speed_y = std::min(other.speed_y, 0.0f);
            }
            break;
        case CollisionType::OVERLAP_LEFT:
            if (touching.right) {
                other.x -= (other_box.x + other_box.w) - me.x;
                other.speed_x = 0.0f;
            } else {
                me.x += (other_box.x + other_box.w) - me.x;
                me.speed_x = 0.0f;
            }
            break;
        case CollisionType::OVERLAP_RIGHT:
            if (touching.left) {
                other.x += (me_box.x + me_box.w) - other_box.x;
                other.speed_x = 0.0f;
            } else {
                me.x -= (me_box.x + me_box.w) - other_box.x;
                me.speed_x = 0.0f;
            }
            break;
        case CollisionType::TOUCH_BOTTOM:
            if (other.collision->on_ground()) {
                touching.bottom = true;
            }
            break;
        default:
            break;
    }
}