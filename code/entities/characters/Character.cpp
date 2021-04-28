#include <SDL2/SDL.h>

#include <utility>
#include <iostream>
#include <optional>

#include "Input.hpp"
#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include "Tiles.hpp"
#include "Assets.hpp"
#include "Level.hpp"

#include "Character.hpp"

/*********************************************
 *              CONSTRUCTORS
 *********************************************/

Character::Character(float x, float y, float w, float h, GraphicsComponent* graphics, InputComponent* input, PhysicsComponent* physics) :
    x(x),
    y(y),
    w(w),
    h(h),
    speed_x(0.0f),
    speed_y(0.0f),
    knockback_speed_x(0.0f),
    knockback_speed_y(0.0f),
    facing_left(false),
    health(3),
    time_last_hit(0),
    graphics(graphics),
    input(input),
    physics(physics)
{
    states.push(CharacterState::MOVING);
}

Character::Character(Character&& other) :
    x(other.x),
    y(other.y),
    w(other.w),
    h(other.h),
    speed_x(other.speed_x),
    speed_y(other.speed_y),
    knockback_speed_x(other.knockback_speed_x),
    knockback_speed_y(other.knockback_speed_y),
    facing_left(other.facing_left),
    health(other.health),
    time_last_hit(other.time_last_hit),
    states(std::move(other.states)),
    graphics(std::exchange(other.graphics, nullptr)),
    input(std::exchange(other.input, nullptr)),
    physics(std::exchange(other.physics, nullptr))    
{}

Character::~Character() {
    delete graphics;
    delete input;
    delete physics;
}

/*********************************************
 *              METHODS
 *********************************************/

void Character::paint() {
    graphics->paint(*this);
}

void Character::handle_event(InputEvent e) {
    if (current_state() == CharacterState::DEAD) { return; }
    input->handle_event(*this, e);
}

// Check if we can hit the other box using the hit box of the current attack frame. If there isn't one, then it returns false.
bool Character::can_hit_now(SDL_Rect other) {
    std::optional<SDL_Rect> my_attack_box = graphics->attack.get_current_collision(x, y, facing_left);
    return my_attack_box && SDL_HasIntersection(&*my_attack_box, &other) == SDL_TRUE;
}

// Check if we can hit the other box using the hit box of some random attack frame.
bool Character::could_hit_sometime(SDL_Rect other) {
    SDL_Rect my_attack_box = graphics->attack.get_any_collision(x, y, facing_left);
    return SDL_HasIntersection(&my_attack_box, &other) == SDL_TRUE;
}

void Character::handle_event(LevelEvent event, Level& level, Character& other) {
    if (&other == this) { return; }
    if (current_state() == CharacterState::DEAD) { return; }

    switch(event) {
        case LevelEvent::ATTACKED:
            if (other.can_hit_now(bounding_box()) && SDL_GetTicks() - time_last_hit > 500) {
                time_last_hit = SDL_GetTicks();
                health--; 
                physics->knockback(*this, other.x);
                assert(health >= 0);
                if (health == 0) {
                    change_state(CharacterState::DEAD);
                    physics->die(*this);
                } else {
                    change_state(CharacterState::HIT);
                    level.send_event(LevelEvent::LOST_HEALTH, *this);
                }
            }
            break;
        default:
            break;
    }
}

void Character::tick(Level& level) {
    if (current_state() != CharacterState::DEAD) {
        input->tick(*this, level);
    }
    physics->tick(*this);
    if (current_state() == CharacterState::ATTACK && graphics->attack.has_collision()) {
        level.send_event(LevelEvent::ATTACKED, *this);
    }
}

void Character::collide_movable(Character& other) {
    physics->collide_movable(*this, other);
}

void Character::collide_immovable(const SDL_Rect& other) {
    physics->collide_immovable(*this, other);
}

CharacterState Character::current_state() {
    assert(!states.empty());
    return states.top();
}

SDL_Rect Character::bounding_box() {
    return { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
}

bool Character::should_be_deleted() {
    return current_state() == CharacterState::DEAD && graphics->dead.loops_completed() && graphics->dead.time_elapsed() > 5000;
}

void Character::change_state(CharacterState state) {
    states.push(state);
    graphics->get_current_sprite(*this).reset_time();
}

/*********************************************
 *              FACTORY METHODS
 *********************************************/

Character* create_player(int x, int y) {
    float w = 16.0f, h = 28.0f;
    Character* player = new Character(
        x, y,
        w, h,    
        new GraphicsComponent(
            AnimatedSprite(Assets::instance().player_run),
            AnimatedSprite(Assets::instance().player_idle),
            AnimatedSprite(Assets::instance().player_attack),
            AnimatedSprite(Assets::instance().player_jump),
            AnimatedSprite(Assets::instance().player_fall),
            AnimatedSprite(Assets::instance().player_ground),
            AnimatedSprite(Assets::instance().player_dead, 10, false),
            AnimatedSprite(Assets::instance().player_hit)
        ),
        new PlayerInputComponent(),
        new PhysicsComponent()
    );
    return player;
}

Character* create_pig(int x, int y, int left_boundary, int right_boundary) {
    float w = 15.0f, h = 16.0f;
    Character* pig = new Character(
        x, y,
        w, h,     
        new GraphicsComponent(
            AnimatedSprite(Assets::instance().pig_run),
            AnimatedSprite(Assets::instance().pig_idle),
            AnimatedSprite(Assets::instance().pig_attack),
            AnimatedSprite(Assets::instance().pig_jump),
            AnimatedSprite(Assets::instance().pig_fall),
            AnimatedSprite(Assets::instance().pig_ground),
            AnimatedSprite(Assets::instance().pig_dead, 10, false),
            AnimatedSprite(Assets::instance().pig_hit)
        ),
        new PigInputComponent(left_boundary, right_boundary),
        new PhysicsComponent()
    );
    return pig;
}