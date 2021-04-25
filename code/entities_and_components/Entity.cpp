#include <SDL2/SDL.h>

#include <utility>
#include <iostream>
#include <optional>

#include "Input.hpp"
#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include "Tiles.hpp"

#include "Entity.hpp"

/*********************************************
 *              CONSTRUCTORS
 *********************************************/

Entity::Entity(float x, float y, float w, float h, GraphicsComponent* graphics, InputComponent* input, PhysicsComponent* physics) :
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
    states.push(EntityState::MOVING);
}

Entity::Entity(Entity&& other) :
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
    graphics(std::exchange(other.graphics, nullptr)),
    input(std::exchange(other.input, nullptr)),
    physics(std::exchange(other.physics, nullptr))    
{}

Entity::~Entity() {
    delete graphics;
    delete input;
    delete physics;
}

/*********************************************
 *              METHODS
 *********************************************/

void Entity::paint() {
    graphics->paint(*this);
}

void Entity::handle_event(InputEvent e) {
    if (current_state() == EntityState::DEAD) { return; }
    input->handle_event(*this, e);
}

// Check if we can hit the other box using the hit box of the current attack frame. If there isn't one, then it returns false.
bool Entity::can_hit_now(SDL_Rect other) {
    std::optional<SDL_Rect> my_attack_box = graphics->attack.get_current_collision(x, y, facing_left);
    return my_attack_box && SDL_HasIntersection(&*my_attack_box, &other) == SDL_TRUE;
}

// Check if we can hit the other box using the hit box of some random attack frame.
bool Entity::could_hit_sometime(SDL_Rect other) {
    SDL_Rect my_attack_box = graphics->attack.get_any_collision(x, y, facing_left);
    return SDL_HasIntersection(&my_attack_box, &other) == SDL_TRUE;
}

void Entity::handle_event(LevelEvent event, Entity* other) {
    if (other == this) { return; }
    if (current_state() == EntityState::DEAD) { return; }

    switch(event) {
        case LevelEvent::ATTACKED:
            if (other->can_hit_now(bounding_box()) && SDL_GetTicks() - time_last_hit > 500) {
                time_last_hit = SDL_GetTicks();
                health--; 
                physics->knockback(*this, other->x);
                if (health <= 0) {
                    change_state(EntityState::DEAD);
                    physics->die(*this);
                } else {
                    change_state(EntityState::HIT);
                }
            }
            break;
        default:
            break;
    }
}

void Entity::tick(Level& level) {
    if (current_state() != EntityState::DEAD) {
        input->tick(*this, level);
    }
    physics->tick(*this);
    if (current_state() == EntityState::ATTACK && graphics->attack.has_collision()) {
        level.send_event(LevelEvent::ATTACKED, this);
    }
}

void Entity::collide_movable(Entity& other) {
    physics->collide_movable(*this, other);
}

void Entity::collide_immovable(const SDL_Rect& other) {
    physics->collide_immovable(*this, other);
}

EntityState Entity::current_state() {
    assert(!states.empty());
    return states.top();
}

SDL_Rect Entity::bounding_box() {
    return { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
}

bool Entity::should_be_deleted() {
    return current_state() == EntityState::DEAD && graphics->dead.loops_completed() && graphics->dead.time_elapsed() > 5000;
}

void Entity::change_state(EntityState state) {
    states.push(state);
    graphics->get_current_sprite(*this).reset_time();
}

/*********************************************
 *              FACTORY METHODS
 *********************************************/

Entity* create_player(int x, int y) {
    float w = 16.0f;
    float h = 28.0f;
    Entity* player = new Entity(
        x, y,
        w, h,    
        new GraphicsComponent(
            AnimatedSprite("assets/images/playerrun.png", 624, 58, 78, 58, 16, 28, 23, 16, 10),
            AnimatedSprite("assets/images/playeridle.png", 858, 58, 78, 58, 16, 28, 23, 16, 10),
            AnimatedSprite("assets/images/playerattack.png", 234, 58, 78, 58, 16, 28, 23, 16, 10),
            AnimatedSprite("assets/images/playerjump.png", 78, 58, 78, 58, 16, 28, 23, 16, 10),
            AnimatedSprite("assets/images/playerfall.png", 78, 58, 78, 58, 16, 28, 23, 16, 10),
            AnimatedSprite("assets/images/playerground.png", 78, 58, 78, 58, 16, 28, 23, 16, 10),
            AnimatedSprite("assets/images/playerdead.png", 312, 58, 78, 58, 16, 28, 23, 16, 10, false, false),
            AnimatedSprite("assets/images/playerhit.png", 156, 58, 78, 58, 16, 28, 23, 16, 10)
        ),
        new PlayerInputComponent(),
        new PhysicsComponent()
    );
    player->graphics->attack.add_collision(0, { 49, 6, 29, 38 });
    player->graphics->attack.add_collision(1, { 42, 4, 29, 40 });
    return player;
}

Entity* create_pig(int x, int y) {
    float w = 15.0f;
    float h = 16.0f;
    Entity* pig = new Entity(
        x, y,
        w, h,     
        new GraphicsComponent(
            AnimatedSprite("assets/images/pigrun.png", 204, 28, 34, 28, 15, 16, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigidle.png", 374, 28, 34, 28, 15, 16, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigattack.png", 170, 28, 34, 28, 15, 16, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigjump.png", 34, 28, 34, 28, 15, 16, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigfall.png", 34, 28, 34, 28, 15, 16, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigground.png", 34, 28, 34, 28, 15, 16, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigdead.png", 136, 28, 34, 28, 15, 16, 12, 12, 10, true, false),
            AnimatedSprite("assets/images/pighit.png", 68, 28, 34, 28, 15, 16, 12, 12, 10, true)
        ),
        new PigInputComponent(),
        new PhysicsComponent()
    );
    pig->graphics->attack.add_collision(2, { 1, 2, 9, 20 });
    pig->graphics->attack.add_collision(3, { 5, 0, 8, 10 });
    return pig;
}