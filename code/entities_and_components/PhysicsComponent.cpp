#include "SDL2/SDL.h"

#include <algorithm>
#include <utility>
#include <iostream>

#include "Entity.hpp"
#include "GraphicsComponent.hpp"

#include "PhysicsComponent.hpp"

/*********************************************
 *                CONSTANTS
 *********************************************/

constexpr float GRAVITY = 0.55f;

/*********************************************
 *           COLLISION DETECTION
 *********************************************/

// From a's perspective.
std::pair<CollisionType,int> rect_collide_rect(const SDL_Rect& a, const SDL_Rect& b) {
    int top_overlap = abs((b.y + b.h) - a.y);
    int bottom_overlap = abs((a.y + a.h) - b.y);
    int left_overlap = abs((b.x + b.w) - a.x);
    int right_overlap = abs((a.x + a.w) - b.x);

    int mini = std::min({top_overlap, bottom_overlap, left_overlap, right_overlap});

    if (SDL_HasIntersection(&a, &b) == SDL_TRUE) {
        if (mini == top_overlap) {
            return { CollisionType::OVERLAP_TOP, mini };
        } else if (mini == bottom_overlap) {
            return { CollisionType::OVERLAP_BOTTOM, mini };
        } else if (mini == left_overlap) {
            return { CollisionType::OVERLAP_LEFT, mini };
        } else if (mini == right_overlap) {
            return { CollisionType::OVERLAP_RIGHT, mini };
        }
    } else if (mini == 0) {
        if (mini == top_overlap) {
            return { CollisionType::TOUCH_TOP, mini };
        } else if (mini == bottom_overlap) {
            return { CollisionType::TOUCH_BOTTOM, mini };
        } else if (mini == left_overlap) {
            return { CollisionType::TOUCH_LEFT, mini };
        } else if (mini == right_overlap) {
            return { CollisionType::TOUCH_RIGHT, mini };
        }
    }

    return { CollisionType::NONE, mini };
}

/*********************************************
 *              COMPONENTS
 *********************************************/

void PhysicsComponent::tick(Entity& entity) {
    entity.speed_y += GRAVITY;
    entity.x += entity.speed_x;
    entity.y += entity.speed_y;
    touching = { false, false, false, false };
}

void PhysicsComponent::collide_immovable(Entity& me, const SDL_Rect& other) {
    auto [collision_type, penetration] = rect_collide_rect(me.bounding_box(), other);
    switch (collision_type) {
        case CollisionType::OVERLAP_TOP:
            touching.top = true;
            me.y += penetration;
            me.speed_y = std::max(me.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_BOTTOM:
            touching.bottom = true;
            me.y -= penetration;
            if (me.speed_y > 2.0f && me.states.top() != EntityState::ATTACK) { 
                me.states.push(EntityState::GROUND);
                me.graphics->ground.reset_time();
            }
            me.speed_y = std::min(me.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_LEFT:
            touching.left = true;
            me.x += penetration;
            me.speed_x = std::max(me.speed_x, 0.0f);
            break;
        case CollisionType::OVERLAP_RIGHT:
            touching.right = true;
            me.x -= penetration;
            me.speed_x = std::min(me.speed_x, 0.0f);
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

// TODO - Should I be able to push people? Who can push who? The larger/heavier object? Is there friction?
void PhysicsComponent::collide_movable(Entity& me, Entity& other) {
    auto [collision_type, penetration] = rect_collide_rect(me.bounding_box(), other.bounding_box());
    switch (collision_type) {
        case CollisionType::OVERLAP_TOP:
            if (touching.bottom) { // he has to move
                other.y -= penetration;
                other.speed_y = 0.0f;
                other.physics->touching.bottom = true;
            } else if (other.physics->touching.top) { // I have to move
                me.y += penetration;
                me.speed_y = 0.0f;
                touching.top = true;
            } else if (me.speed_y * other.speed_y >= 0.0f) { // moving in the same direction or one is stationary
                me.y += penetration / 2;
                other.y -= penetration / 2;
                me.speed_y = std::min(me.speed_y, other.speed_y);
                other.speed_y = std::min(me.speed_y, other.speed_y);
            } else { // moving towards each other
                me.y += penetration / 2;
                other.y -= penetration / 2;
                me.speed_y = 0.0f;
                other.speed_y = 0.0f;
            }
            break;
        case CollisionType::OVERLAP_BOTTOM:
            if (other.physics->touching.bottom) {
                me.y -= penetration;
                me.speed_y = std::min(me.speed_y, 0.0f);
                touching.bottom = true;
                if (me.speed_y > 2.0f && me.states.top() != EntityState::ATTACK) { 
                    me.states.push(EntityState::GROUND);
                    me.graphics->ground.reset_time();
                }
            } else {
                other.y += penetration;
                other.speed_y = std::min(other.speed_y, 0.0f);
            }
            break;
        case CollisionType::OVERLAP_LEFT:
            if (touching.right) {
                other.x -= penetration;
                other.speed_x = 0.0f;
                me.speed_x = 0.0f;
            } else {
                me.x += penetration;
                me.speed_x = std::min(me.speed_x, other.speed_x);
            }
            break;
        case CollisionType::OVERLAP_RIGHT:
            if (touching.left) {
                other.x += penetration;
                other.speed_x = 0.0f;
            } else {
                me.x -= penetration;
                me.speed_x = 0.0f;
            }
            break;
        case CollisionType::TOUCH_TOP:
            if (touching.bottom) {
                other.physics->touching.bottom = true;
            }
            break;
        case CollisionType::TOUCH_BOTTOM:
            if (other.physics->touching.bottom) {
                touching.bottom = true;
            }
            break;
        case CollisionType::TOUCH_LEFT:
            if (touching.right) {
                other.physics->touching.right = true;
            }
            break;
        case CollisionType::TOUCH_RIGHT:
            if (touching.left) {
                other.physics->touching.left = true;
            }
            break;
        default:
            break;
    }
}