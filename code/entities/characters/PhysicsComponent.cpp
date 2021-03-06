#include "SDL2/SDL.h"

#include <algorithm>
#include <utility>
#include <iostream>
#include <stdexcept>

#include "Character.hpp"
#include "GraphicsComponent.hpp"

#include "PhysicsComponent.hpp"

//--------------------------------------------
//                CONSTANTS
//--------------------------------------------

constexpr float GRAVITY = 0.55f;
constexpr float KNOCKBACK_DECAY = 0.3f;
constexpr float KNOCKBACK_X = 4.5f;
constexpr float KNOCKBACK_Y = 6.5f;

//--------------------------------------------
//           COLLISION DETECTION
//--------------------------------------------

// From a's perspective.
std::pair<CollisionType,int> rect_collide_rect(const SDL_Rect& a, const SDL_Rect& b) {
    int top_overlap = (b.y + b.h) - a.y;
    int bottom_overlap = (a.y + a.h) - b.y;
    int left_overlap = (b.x + b.w) - a.x;
    int right_overlap = (a.x + a.w) - b.x;

    if (top_overlap < 0 || bottom_overlap < 0 || left_overlap < 0 || right_overlap < 0) {
        return { CollisionType::NONE, 0 };
    }

    top_overlap = std::abs(top_overlap);
    bottom_overlap = std::abs(bottom_overlap);
    left_overlap = std::abs(left_overlap);
    right_overlap = std::abs(right_overlap);
    int mini = std::min({top_overlap, bottom_overlap, left_overlap, right_overlap});

    if (mini > 0) {
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

    throw std::runtime_error("Unhandled collision type");
}

//--------------------------------------------
//              COMPONENTS
//--------------------------------------------

void PhysicsComponent::tick(Character& character) {
    character.speed_y += GRAVITY;
    if (character.knockback_speed_x != 0.0f) {
        character.knockback_speed_x = character.knockback_speed_x > 0.0f ? std::max(0.0f, character.knockback_speed_x - KNOCKBACK_DECAY) : std::min(0.0f, character.knockback_speed_x + KNOCKBACK_DECAY);
    }
    if (character.knockback_speed_y != 0.0f) {
        character.knockback_speed_y = character.knockback_speed_y > 0.0f ? std::max(0.0f, character.knockback_speed_y - KNOCKBACK_DECAY) : std::min(0.0f, character.knockback_speed_y + KNOCKBACK_DECAY);
    }
    character.x += character.speed_x + character.knockback_speed_x;
    character.y += character.speed_y + character.knockback_speed_y;
    touching = { false, false, false, false };
}

void PhysicsComponent::die(Character& me) {
    me.speed_x = 0.0f;
}

void PhysicsComponent::knockback(Character& me, float source_x) {
    me.knockback_speed_y -= KNOCKBACK_Y;
    if (source_x < me.x) {
        me.knockback_speed_x += KNOCKBACK_X;
    } else {
        me.knockback_speed_x -= KNOCKBACK_X;
    } 
}

// Collide a movable object (me) against an immovable object (other).
void PhysicsComponent::collide_immovable(Character& me, const SDL_Rect& other) {
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
            if (me.speed_y > 2.0f && me.current_state() == CharacterState::MOVING) { 
                me.change_state(CharacterState::GROUND);
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

// Collide a movable object (me) against a movable object (other).
void PhysicsComponent::collide_movable(Character& me, Character& other) {
    auto [collision_type, penetration] = rect_collide_rect(me.bounding_box(), other.bounding_box());
    switch (collision_type) {
        case CollisionType::OVERLAP_TOP:
            if (touching.bottom) { // he has to move
                other.y -= penetration;
                other.speed_y = 0.0f;
                other.physics->touching.bottom = true;
                if (other.speed_y > 2.0f && other.current_state() != CharacterState::ATTACK) {
                    other.change_state(CharacterState::GROUND);
                }
            } else if (other.physics->touching.top) { // I have to move
                me.y += penetration;
                me.speed_y = 0.0f;
                touching.top = true;
            } else if (me.speed_y * other.speed_y >= 0.0f) { // moving in the same direction or one is stationary
                me.y += penetration / 2;
                other.y -= penetration / 2;
                float smallest = me.speed_y >= 0.0f && other.speed_y >= 0.0f ? std::min(me.speed_y, other.speed_y) : std::max(me.speed_y, other.speed_y);
                me.speed_y = smallest;
                other.speed_y = smallest;
            } else { // moving towards each other
                me.y += penetration / 2;
                other.y -= penetration / 2;
                me.speed_y = 0.0f;
                other.speed_y = 0.0f;
            }
            break;
        case CollisionType::OVERLAP_BOTTOM:
            if (touching.top) { // he has to move
                other.y += penetration;
                other.speed_y = 0.0f;
                other.physics->touching.top = true;
            } else if (other.physics->touching.bottom) { // I have to move
                me.y -= penetration;
                me.speed_y = 0.0f;
                touching.bottom = true;
                if (me.speed_y > 2.0f && me.current_state() != CharacterState::ATTACK) { 
                    me.change_state(CharacterState::GROUND);
                }
            } else if (other.speed_y * me.speed_y >= 0.0f) { // moving in the same direction or one is stationary
                other.y += penetration / 2;
                me.y -= penetration / 2;
                float smallest = me.speed_y >= 0.0f && other.speed_y >= 0.0f ? std::min(me.speed_y, other.speed_y) : std::max(me.speed_y, other.speed_y);
                me.speed_y = smallest;
                other.speed_y = smallest;
            } else { // moving towards each other
                other.y += penetration / 2;
                me.y -= penetration / 2;
                other.speed_y = 0.0f;
                me.speed_y = 0.0f;
            }
            break;
        case CollisionType::OVERLAP_LEFT:
            if (touching.right) { // he has to move
                other.x -= penetration;
                other.speed_x = 0.0f;
                other.physics->touching.right = true;
            } else if (other.physics->touching.left) { // I have to move
                me.x += penetration;
                me.speed_x = 0.0f;
                touching.left = true;
            } else if (me.speed_x * other.speed_x >= 0.0f) { // moving in the same direction or one is stationary
                me.x += penetration / 2;
                other.x -= penetration / 2;
                float smallest = me.speed_x >= 0.0f && other.speed_x >= 0.0f ? std::min(me.speed_x, other.speed_x) : std::max(me.speed_x, other.speed_x);
                me.speed_x = smallest;
                other.speed_x = smallest;
            } else { // moving towards each other
                me.x += penetration / 2;
                other.x -= penetration / 2;
                me.speed_x = 0.0f;
                other.speed_x = 0.0f;
            }
            break;
        case CollisionType::OVERLAP_RIGHT:
            if (touching.left) { // he has to move
                other.x += penetration;
                other.speed_x = 0.0f;
                other.physics->touching.left = true;
            } else if (other.physics->touching.right) { // I have to move
                me.x -= penetration;
                me.speed_x = 0.0f;
                touching.right = true;
            } else if (other.speed_x * me.speed_x >= 0.0f) { // moving in the same direction or one is stationary
                other.x += penetration / 2;
                me.x -= penetration / 2;
                float smallest = other.speed_x >= 0.0f && me.speed_x >= 0.0f ? std::min(other.speed_x, me.speed_x) : std::max(other.speed_x, me.speed_x);
                other.speed_x = smallest;
                me.speed_x = smallest;
            } else { // moving towards each me
                other.x += penetration / 2;
                me.x -= penetration / 2;
                other.speed_x = 0.0f;
                me.speed_x = 0.0f;
            }
            break;
        case CollisionType::TOUCH_TOP:
            if (touching.bottom) {
                other.physics->touching.bottom = true;
            }
            if (other.physics->touching.top) {
                touching.top = true;
            }
            break;
        case CollisionType::TOUCH_BOTTOM:
            if (touching.top) {
                other.physics->touching.top = true;
            }
            if (other.physics->touching.bottom) {
                touching.bottom = true;
            }
            break;
        case CollisionType::TOUCH_LEFT:
            if (touching.right) {
                other.physics->touching.right = true;
            }
            if (other.physics->touching.left) {
                touching.left = true;
            }
            break;
        case CollisionType::TOUCH_RIGHT:
            if (touching.left) {
                other.physics->touching.left = true;
            }
            if (other.physics->touching.right) {
                touching.right = true;
            }
            break;
        default:
            break;
    }
}