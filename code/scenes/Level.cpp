#include <algorithm>

#include "Camera.hpp"
#include "Entity.hpp"
#include "GraphicsComponent.hpp"

#include "Level.hpp"

Level::Level() :
    level("assets/level.tmx")
{
    player = create_player(level.markers.at("PlayerSpawn").x, level.markers.at("PlayerSpawn").y);
    pigs.push_back(create_pig(level.markers.at("PigSpawn").x, level.markers.at("PigSpawn").y));
}

Level::~Level() {
    delete player;
    for (auto& pig : pigs) { 
        delete pig; 
    }
}

void Level::handle_event(InputEvent e) {
    player->handle_event(e);
    for (auto& pig : pigs) { 
        pig->handle_event(e); 
    }
}

void Level::tick() {
    // update position
    player->tick(*this);
    for (auto& pig : pigs) { 
        pig->tick(*this); 
    }
    // collide each object with level
    for (auto& c : level.collisions) {
        player->collide_immovable(c);
        for (auto& pig : pigs) { 
            pig->collide_immovable(c); 
        }
    }
    // collide each pair of objects
    for (auto& pig : pigs) { 
        player->collide_movable(*pig); 
    }
    // update camera
    Camera::tick(static_cast<int>(player->x));
    // remove any dead objects
    std::remove_if(pigs.begin(), pigs.end(), [](Entity* p){ return !p->is_alive; });
    // TODO - memory leak because delete is not called
}

void Level::paint() {
    level.paint();
    player->paint();
    for (auto& pig : pigs) { 
        pig->paint(); 
    }
}

void Level::send_event(LevelEvent event, Entity* entity) {
    player->handle_event(event, entity);
    for (auto& pig : pigs) { 
        pig->handle_event(event, entity);
    }
    // TODO
}