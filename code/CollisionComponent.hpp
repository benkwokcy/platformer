#pragma once

class Player;

class CollisionComponent {
public:
    void collide_map(Player& player, const SDL_Rect& other);
};