#include "Entity.hpp"
#include "Input.hpp"
#include "GraphicsComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Tiles.hpp"

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
            if (me.states.top() != EntityState::ATTACK) {
                me.states.push(EntityState::ATTACK);
                me.graphics->attack.reset_time();
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

void PlayerInputComponent::tick(Entity& me, Tilemap& level) {
    if (Input::left_down() || Input::right_down()) {
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

void PigInputComponent::handle_event(Entity& me, InputEvent e) {}

void PigInputComponent::tick(Entity& me, Tilemap& level) {
    if (me.x <= level.markers.at("PigLeftBoundary").x || me.speed_x == 0.0f) {
        if (me.physics->touching.right && me.physics->on_ground()) {
            me.speed_y -= 11.0f;
        }
        me.facing_left = false;
        me.speed_x = 2.0f;
    } else if (me.x >= level.markers.at("PigRightBoundary").x) {
        if (me.physics->touching.left && me.physics->on_ground()) {
            me.speed_y -= 11.0f;
        }
        me.facing_left = true;
        me.speed_x = -2.0f;
    }
}