#pragma once

#include <stack>

#include "Input.hpp"

struct SDL_Rect;
class GraphicsComponent;
class InputComponent;
class PhysicsComponent;
class PhysicsComponent;

enum class EntityState {
    MOVING, ATTACK, GROUND
};

class Entity {
public:
    float x, y; // the top left corner of the entity in level coordinates
    float w, h; // dimensions of the bounding box
    float speed_x, speed_y;
    bool facing_left;
    std::stack<EntityState> states;
    GraphicsComponent* graphics;
    InputComponent* input;
    PhysicsComponent* physics;

    Entity(float x, float y, float w, float h, GraphicsComponent* graphics, InputComponent* input, PhysicsComponent* physics);
    ~Entity();
    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) = delete;
    Entity(Entity&& other);
    Entity& operator=(Entity&& other) = delete;

    void paint();
    void handle_event(InputEvent e);
    void tick();
    void collide_immovable(const SDL_Rect& other);
    void collide_movable(Entity& other);
    SDL_Rect bounding_box();
};

Entity* create_player(int x, int y);
Entity* create_pig(int x, int y);