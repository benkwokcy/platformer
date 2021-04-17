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
                entity.graphics->attack.reset_to_first_frame();
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