#include "Player.hpp"
#include "Input.hpp"
#include "GraphicsComponent.hpp"

#include "InputComponent.hpp"

void InputComponent::handle_event(Player& player, InputEvent e) {
    switch (e) {
        case InputEvent::LEFT:
            player.facing_left = true;
            break;
        case InputEvent::RIGHT:
            player.facing_left = false;
            break;
        case InputEvent::ATTACK:
            if (player.states.top() != PlayerState::ATTACK) {
                player.states.push(PlayerState::ATTACK);
                player.graphics->attack.reset_to_first_frame();
            }
            break;
        case InputEvent::JUMP:
            if (player.on_ground) {
                player.speed_y -= 11.0f;
            }
        default:
            break;
    }
}