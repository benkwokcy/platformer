#include <algorithm>
#include <iostream>

#include "Camera.hpp"
#include "Entity.hpp"
#include "GraphicsComponent.hpp"
#include "Assets.hpp"

#include "Level.hpp"

Level::Level() :
    level(Assets::PATH + "level.tmx")
{
    player = create_player(level.markers.at("PlayerSpawn").x, level.markers.at("PlayerSpawn").y);
    pigs.push_back(create_pig(level.markers.at("PigSpawn1").x, level.markers.at("PigSpawn1").y, level.markers.at("PigLeftBoundary1").x, level.markers.at("PigRightBoundary1").x));
    pigs.push_back(create_pig(level.markers.at("PigSpawn2").x, level.markers.at("PigSpawn2").y, level.markers.at("PigLeftBoundary2").x, level.markers.at("PigRightBoundary2").x));
}

Level::~Level() {
    delete player;
    for (auto pig : pigs) { 
        delete pig; 
    }
}

void Level::handle_event(InputEvent e) {
    player->handle_event(e);
}

void Level::tick() {
    // update each object one frame
    player->tick(*this);
    for (auto pig : pigs) { 
        pig->tick(*this);   
    }
    // collide each object with the level
    for (auto& c : level.collisions) {
        player->collide_immovable(c);
        for (auto& pig : pigs) { 
            pig->collide_immovable(c); 
        }
    }
    // collide each pair of objects
    for (auto pig : pigs) { 
        player->collide_movable(*pig); 
        // TODO - collide pigs with each other
    }
    // update camera
    Camera::tick(static_cast<int>(player->x));
    // remove any dead objects
    for (int i = pigs.size() - 1; i >= 0; i--) {
        if (pigs[i]->should_be_deleted()) {
            delete pigs[i];
            pigs.erase(pigs.begin() + i);
        }
    }
}

void Level::paint() {
    level.paint();
    player->paint();
    for (auto pig : pigs) { 
        pig->paint(); 
    }
    healthbar.paint(player->health);
}

void Level::send_event(LevelEvent event, Entity& entity) {
    player->handle_event(event, *this, entity);
    for (auto pig : pigs) { 
        pig->handle_event(event, *this, entity);
    }
    healthbar.handle_event(event, *this, entity);
}