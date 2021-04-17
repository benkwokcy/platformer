#include "Input.hpp"
#include "CollisionComponent.hpp"
#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"

#include "Entity.hpp"

/*********************************************
 *              CONSTRUCTORS
 *********************************************/

Entity::Entity(float x, float y) :
    x(x),
    y(y),
    w(16.0f),
    h(28.0f),
    speed_x(0.0f),
    speed_y(0.0f),
    facing_left(false),
    on_ground(false),
    graphics(new GraphicsComponent()),
    input(new InputComponent()),
    collision(new CollisionComponent()),
    physics(new PhysicsComponent())
{
    states.push(EntityState::MOVING);
}

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
    physics->tick(*this);
}

void Entity::collide_map(const SDL_Rect& other) {
    collision->collide_map(*this, other);
}

SDL_Rect Entity::bounding_box() {
    return { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
}

/*********************************************
 *              FACTORY METHODS
 *********************************************/

Entity* create_player(int x, int y) {
    return new Entity(x, y);
}