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

enum class CharacterState {
    MOVING, ATTACK, GROUND, DEAD, HIT
};

class Character {
public:
    // Constructors
    Character(float x, float y, float w, float h, GraphicsComponent* graphics, InputComponent* input, PhysicsComponent* physics);
    ~Character();
    Character(const Character& other) = delete;
    Character& operator=(const Character& other) = delete;
    Character(Character&& other);
    Character& operator=(Character&& other) = delete;

    // Methods
    void paint();
    void handle_event(InputEvent e);
    void handle_event(LevelEvent event, Level& level, Character& other);
    void tick(Level& level);
    void collide_immovable(const SDL_Rect& other);
    void collide_movable(Character& other);
    CharacterState current_state();
    SDL_Rect bounding_box();
    bool should_be_deleted(Level& level);
    void change_state(CharacterState state);
    bool can_hit_now(SDL_Rect other);
    bool could_hit_sometime(SDL_Rect other);

    // Member Variables
    float x, y; // the top left corner of the character in level coordinates
    float w, h; // dimensions of the bounding box
    float speed_x, speed_y;
    float knockback_speed_x, knockback_speed_y;
    bool facing_left;
    int health;
    int time_last_hit;
    std::stack<CharacterState> states;
    GraphicsComponent* graphics;
    InputComponent* input;
    PhysicsComponent* physics;
};

Character* create_player(int x, int y);
Character* create_pig(int x, int y, int left_boundary, int right_boundary);