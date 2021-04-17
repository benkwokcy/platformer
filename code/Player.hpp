#pragma once

#include <SDL2/SDL.h>

#include <stack>

#include "Entity.hpp"
#include "Input.hpp"

struct SDL_Rect;
class GraphicsComponent;
class InputComponent;
class CollisionComponent;
class PhysicsComponent;

enum class PlayerState {
    MOVING, ATTACK
};

class Player : public Entity {
public:
    float x, y; // the top left corner of the player in level coordinates
    float w, h; // dimensions of the bounding box
    float speed_x, speed_y;
    bool facing_left, on_ground;
    std::stack<PlayerState> states;
    GraphicsComponent* graphics;
    InputComponent* input;
    CollisionComponent* collision;
    PhysicsComponent* physics;

    Player(float x, float y);
    ~Player();
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) = delete;
    Player& operator=(Player&& other) = delete;

    void paint() override;
    void handle_event(InputEvent e) override;
    void tick() override;
    void collide_map(const SDL_Rect& other);
    SDL_Rect bounding_box();
};