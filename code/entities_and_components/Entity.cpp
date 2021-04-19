#include <utility>

#include "Input.hpp"
#include "CollisionComponent.hpp"
#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"

#include "Entity.hpp"

/*********************************************
 *              CONSTRUCTORS
 *********************************************/

Entity::Entity(float x, float y, float w, float h, GraphicsComponent* graphics, InputComponent* input, CollisionComponent* collision, PhysicsComponent* physics) :
    x(x),
    y(y),
    w(w),
    h(h),
    speed_x(0.0f),
    speed_y(0.0f),
    facing_left(false),
    on_ground(false),
    graphics(graphics),
    input(input),
    collision(collision),
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
    facing_left(other.facing_left),
    on_ground(other.on_ground),
    graphics(std::exchange(other.graphics, nullptr)),
    input(std::exchange(other.input, nullptr)),
    collision(std::exchange(other.collision, nullptr)),
    physics(std::exchange(other.physics, nullptr))    
{}

Entity::~Entity() {
    delete graphics;
    delete input;
    delete collision;
    delete physics;
}

/*********************************************
 *              METHODS
 *********************************************/

void Entity::paint() {
    graphics->paint(*this);
}

void Entity::handle_event(InputEvent e) {
    input->handle_event(*this, e);
}

void Entity::tick() {
    input->tick(*this);
    physics->tick(*this);
}

void Entity::collide(const SDL_Rect& other) {
    collision->collide(*this, other);
}

SDL_Rect Entity::bounding_box() {
    return { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
}

/*********************************************
 *              FACTORY METHODS
 *********************************************/

Entity* create_player(int x, int y) {
    float w = 16.0f;
    float h = 28.0f;
    return new Entity(
        x, y,
        w, h,    
        new GraphicsComponent(
            AnimatedSprite("assets/images/playerrun.png", 624, 58, 78, 58, w, h, 23, 16, 10),
            AnimatedSprite("assets/images/playeridle.png", 858, 58, 78, 58, w, h, 23, 16, 10),
            AnimatedSprite("assets/images/playerattack.png", 234, 58, 78, 58, w, h, 23, 16, 10),
            AnimatedSprite("assets/images/playerjump.png", 78, 58, 78, 58, w, h, 23, 16, 10),
            AnimatedSprite("assets/images/playerfall.png", 78, 58, 78, 58, w, h, 23, 16, 10),
            AnimatedSprite("assets/images/playerground.png", 78, 58, 78, 58, w, h, 23, 16, 10)
        ),
        new InputComponent(),
        new CollisionComponent(),
        new PhysicsComponent()
    );
}

Entity* create_pig(int x, int y) {
    float w = 15.0f;
    float h = 16.0f;
    return new Entity(
        x, y,
        w, h,     
        new GraphicsComponent(
            AnimatedSprite("assets/images/pigrun.png", 204, 28, 34, 28, w, h, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigidle.png", 374, 28, 34, 28, w, h, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigattack.png", 170, 28, 34, 28, w, h, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigjump.png", 34, 28, 34, 28, w, h, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigfall.png", 34, 28, 34, 28, w, h, 12, 12, 10, true),
            AnimatedSprite("assets/images/pigground.png", 34, 28, 34, 28, w, h, 12, 12, 10, true)
        ),
        new InputComponent(),
        new CollisionComponent(),
        new PhysicsComponent()
    );
}