#pragma once

#include <stack>

#include "Input.hpp"
#include "LevelEvent.hpp"

struct SDL_Rect;
class GraphicsComponent;
class InputComponent;
class PhysicsComponent;
class PhysicsComponent;
class Tilemap;
class Level;

enum class EntityState {
    MOVING, ATTACK, GROUND, DEAD, HIT
};

class Entity {
public:
    float x, y; // the top left corner of the entity in level coordinates
    float w, h; // dimensions of the bounding box
    float speed_x, speed_y;
    float knockback_speed_x, knockback_speed_y;
    bool facing_left;
    int health;
    int time_last_hit;
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
    void handle_event(LevelEvent event, Level* level, Entity* other);
    void tick(Level& level);
    void collide_immovable(const SDL_Rect& other);
    void collide_movable(Entity& other);
    EntityState current_state();
    SDL_Rect bounding_box();
    bool should_be_deleted();
    void change_state(EntityState state);
    bool can_hit_now(SDL_Rect other);
    bool could_hit_sometime(SDL_Rect other);
};

Entity* create_player(int x, int y);
Entity* create_pig(int x, int y, int left_boundary, int right_boundary);