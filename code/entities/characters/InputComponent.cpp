#include "Character.hpp"
#include "Input.hpp"
#include "GraphicsComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Tiles.hpp"
#include "Level.hpp"

#include "InputComponent.hpp"

/*********************************************
 *           PlayerInputComponent
 *********************************************/

void PlayerInputComponent::handle_event(Character& me, InputEvent e) {
    switch (e) {
        case InputEvent::LEFT:
            me.facing_left = true;
            break;
        case InputEvent::RIGHT:
            me.facing_left = false;
            break;
        case InputEvent::ATTACK:
            if (me.current_state() != CharacterState::ATTACK) {
                me.change_state(CharacterState::ATTACK);
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

void PlayerInputComponent::tick(Character& me, Level& level) {
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

void PigInputComponent::handle_event(Character& me, InputEvent e) {}

void PigInputComponent::tick(Character& me, Level& level) {
    if (me.current_state() == CharacterState::ATTACK) {
        return;
    } else if (me.current_state() != CharacterState::ATTACK && level.player->health > 0 && me.could_hit_sometime(level.player->bounding_box())) {
        me.change_state(CharacterState::ATTACK);
    } else if (me.x <= left_boundary) {
        if (me.physics->touching.right && me.physics->on_ground()) {
            me.speed_y -= 11.0f;
        }
        me.facing_left = false;
        me.speed_x = 2.0f;
    } else if (me.x >= right_boundary) {
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