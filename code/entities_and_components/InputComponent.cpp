#include "Entity.hpp"
#include "Input.hpp"
#include "GraphicsComponent.hpp"

#include "InputComponent.hpp"

void InputComponent::handle_event(Entity& entity, InputEvent e) {
    switch (e) {
        case InputEvent::LEFT:
            entity.facing_left = true;
            break;
        case InputEvent::RIGHT:
            entity.facing_left = false;
            break;
        case InputEvent::ATTACK:
            if (entity.states.top() != EntityState::ATTACK) {
                entity.states.push(EntityState::ATTACK);
                entity.graphics->attack.reset_time();
            }
            break;
        case InputEvent::JUMP:
            if (entity.on_ground) {
                entity.speed_y -= 11.0f;
            }
        default:
            break;
    }
}

void InputComponent::tick(Entity& entity) {
    if (Input::left_down() || Input::right_down()) {
        if (entity.facing_left) {
            entity.speed_x = -4.0f;
        } else {
            entity.speed_x = 4.0f;
        }
    } else {
        entity.speed_x = 0.0f;
    }
}