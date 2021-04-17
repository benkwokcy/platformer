#pragma once

class Player;

namespace Physics {
    constexpr float GRAVITY = 0.55f;
};

class PhysicsComponent {
public:
    void tick(Player& player);
};
