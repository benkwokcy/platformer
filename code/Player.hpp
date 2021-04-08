#pragma once

#include <stack>

#include "Entity.hpp"
#include "Event.hpp"
#include "PlayerState.hpp"

class Player : public Entity {
public:
    PlayerRunLeft run_left;
    PlayerRunRight run_right;
    PlayerIdle idle;
    PlayerAttack attack;

    stack<PlayerState*> state;
    bool facing_left;
    float speed_x;
    float speed_y;
    float x;
    float y;

    Player();
    void paint() override;
    void handle_event(Event e) override;
    void tick() override;
};