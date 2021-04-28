#include "Entity.hpp"
#include "Input.hpp"
#include "GraphicsComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Tiles.hpp"
#include "Level.hpp"

#include "InputComponent.hpp"

/*********************************************
 *           PlayerInputComponent
 *********************************************/

void PlayerInputComponent::handle_event(Entity& me, InputEvent e) {
    switch (e) {
        case InputEvent::LEFT:
            me.facing_left = true;
            break;
        case InputEvent::RIGHT:
            me.facing_left = false;
            break;
        case InputEvent::ATTACK:
            if (me.current_state() != EntityState::ATTACK) {
                me.change_state(EntityState::ATTACK);
            }
            break;
        case InputEvent::JUMP:
            if (me.physics->on_ground()) {
                me.speed_y -= 11.0f;
            }
        default:
            break;
    }
}

void PlayerInputComponent::tick(Entity& me, Level& level) {
    if (Input::is_left_down() || Input::is_right_down()) {
        if (me.facing_left) {
            me.speed_x = -4.0f;
        } else {
            me.speed_x = 4.0f;
        }
    } else {
        me.speed_x = 0.0f;
    }
}

/*********************************************
 *           PigInputComponent
 *********************************************/

PigInputComponent::PigInputComponent(int left_boundary, int right_boundary) :
    left_boundary(left_boundary),
    right_boundary(right_boundary)
{}

void PigInputComponent::handle_event(Entity& me, InputEvent e) {}

void PigInputComponent::tick(Entity& me, Level& level) {
    if (me.current_state() == EntityState::ATTACK) {
        return;
    } else if (me.current_state() != EntityState::ATTACK && level.player->health > 0 && me.could_hit_sometime(level.player->bounding_box())) {
        me.change_state(EntityState::ATTACK);
    } else if (me.x <= left_boundary || me.physics->touching.left) {
        if (me.physics->touching.right && me.physics->on_ground()) {
            me.speed_y -= 11.0f;
        }
        me.facing_left = false;
        me.speed_x = 2.0f;
    } else if (me.x >= right_boundary || me.physics->touching.right) {
        if (me.physics->touching.left && me.physics->on_ground()) {
            me.speed_y -= 11.0f;
        }
        me.facing_left = true;
        me.speed_x = -2.0f;
    } else {
        if (me.facing_left) {
            me.speed_x = -2.0f;
        } else {
            me.speed_x = 2.0f;
        }
    }
}