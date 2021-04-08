#include "PlayerState.hpp"
#include "Player.hpp"
#include "Event.hpp"
#include "Sprite.hpp"

/********************* PlayerRunLeft *********************/

PlayerRunLeft::PlayerRunLeft() :
    sprite(Sprite("sprites/playerrun.png", 78, 58, 8, 10))
{}

void PlayerRunLeft::enter(Player& player) {
    sprite.set_first_frame();
    player.facing_left = true;
    player.speed_x = -0.5f;
}

bool PlayerRunLeft::handle_event(Player& player, Event e) {
    switch (e) {
        case Event::RIGHT_PRESS:
            player.state.pop();
            player.state.push(&(player.run_right));
            return true;
            break;
        case Event::LEFT_RELEASE:
            player.state.pop();
            return true;
            break;
        case Event::ATTACK:
            player.state.push(&(player.attack));
            return true;
            break;
        default:
            return false;
    }
}

void PlayerRunLeft::paint(Player& player) {
    sprite.paint(player.x, player.y, player.facing_left);
}

void PlayerRunLeft::tick(Player& player) {}

/********************* PlayerRunRight *********************/

PlayerRunRight::PlayerRunRight() :
    sprite(Sprite("sprites/playerrun.png", 78, 58, 8, 10))
{}

void PlayerRunRight::enter(Player& player) {
    sprite.set_first_frame();
    player.facing_left = false;
    player.speed_x = 0.5f;
}

bool PlayerRunRight::handle_event(Player& player, Event e) {
    switch (e) {
        case Event::LEFT_PRESS:
            player.state.pop();
            player.state.push(&(player.run_left));
            return true;
            break;
        case Event::RIGHT_RELEASE:
            player.state.pop();
            return true;
            break;
        case Event::ATTACK:
            player.state.push(&(player.attack));
            return true;
            break;
        default:
            return false;
    }
}

void PlayerRunRight::paint(Player& player) {
    sprite.paint(player.x, player.y, player.facing_left);
}

void PlayerRunRight::tick(Player& player) {}

/********************* PlayerIdle *********************/

PlayerIdle::PlayerIdle() :
    sprite((Sprite("sprites/playeridle.png", 78, 58, 11, 10)))
{}

void PlayerIdle::enter(Player& player) {
    sprite.set_first_frame();
    player.speed_x = 0.0f;
}

bool PlayerIdle::handle_event(Player& player, Event e) {
    switch (e) {
        case Event::LEFT_PRESS:
            player.state.push(&(player.run_left));
            return true;
            break;
        case Event::RIGHT_RELEASE:
            player.state.push(&(player.run_right));
            return true;
            break;
        case Event::ATTACK:
            player.state.push(&(player.attack));
            return true;
            break;
        default:
            return false;
    }
}

void PlayerIdle::paint(Player& player) {
    sprite.paint(player.x, player.y, player.facing_left);
}

void PlayerIdle::tick(Player& player) {}

/********************* PlayerAttack *********************/

PlayerAttack::PlayerAttack() :
    sprite(Sprite("sprites/playerattack.png", 78, 58, 3, 10))
{}

void PlayerAttack::enter(Player& player) {
    sprite.set_first_frame();
    player.speed_x = 0.0f;
}

bool PlayerAttack::handle_event(Player& player, Event e) {
    switch (e) {
        case Event::LEFT_PRESS:
            player.state.push(&(player.run_left));
            return true;
            break;
        case Event::RIGHT_RELEASE:
            player.state.push(&(player.run_right));
            return true;
            break;
        case Event::ATTACK:
            player.state.push(&(player.attack));
            return true;
            break;
        default:
            return false;
    }
}

void PlayerAttack::paint(Player& player) {
    sprite.paint(player.x, player.y, player.facing_left);
}

void PlayerAttack::tick(Player& player) {
    if (sprite.frames_elapsed() >= sprite.num_frames) {
        player.state.pop();
    }
}