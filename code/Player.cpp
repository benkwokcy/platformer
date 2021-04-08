#include <stack>

#include "Player.hpp"
#include "PlayerState.hpp"
#include "Window.hpp"
#include "Event.hpp"

Player::Player() :
    x(Window::center_x()),
    y(Window::center_y()),
    speed_x(0.0f),
    speed_y(0.0f),
    facing_left(false)
{
    state.push(&idle);
}

void Player::paint() {
    assert(!state.empty());
    state.top()->paint(*this);
}

void Player::handle_event(Event e) {
    assert(!state.empty());
    bool stateChanged = state.top()->handle_event(*this, e);
    assert(!state.empty());
    if (stateChanged) {
        state.top()->enter(*this);
    }
}

void Player::tick() {
    x += speed_x;
    y += speed_y;
}