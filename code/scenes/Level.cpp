#include <algorithm>
#include <iostream>

#include "Camera.hpp"
#include "Character.hpp"
#include "GraphicsComponent.hpp"
#include "Assets.hpp"

#include "Level.hpp"

Level::Level() :
    level(Assets::PATH + "level.tmx"),
    start_door(level.markers.at("StartDoor").x, level.markers.at("StartDoor").y),
    end_door(level.markers.at("EndDoor").x, level.markers.at("EndDoor").y)
{
    player = create_player(level.markers.at("PlayerSpawn").x, level.markers.at("PlayerSpawn").y);
    pigs.push_back(create_pig(level.markers.at("PigSpawn1").x, level.markers.at("PigSpawn1").y, level.markers.at("PigLeftBoundary1").x, level.markers.at("PigRightBoundary1").x));
    pigs.push_back(create_pig(level.markers.at("PigSpawn2").x, level.markers.at("PigSpawn2").y, level.markers.at("PigLeftBoundary2").x, level.markers.at("PigRightBoundary2").x));
    pigs.push_back(create_pig(level.markers.at("PigSpawn3").x, level.markers.at("PigSpawn3").y, level.markers.at("PigLeftBoundary3").x, level.markers.at("PigRightBoundary3").x));
}

Level::~Level() {
    delete player;
    for (auto pig : pigs) { 
        delete pig; 
    }
}

void Level::enter() {
    start_door.open();
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
    // collide each character with the level
    for (auto& c : level.collisions) {
        player->collide_immovable(c);
        for (auto& pig : pigs) { 
            pig->collide_immovable(c); 
        }
    }
    // collide each pair of characters
    for (auto pig : pigs) { 
        player->collide_movable(*pig); 
        // TODO - collide pigs with each other
    }
    // if player is dead, respawn player
    if (player->should_be_deleted(*this)) {
        delete player;
        player = create_player(level.markers.at("PlayerSpawn").x, level.markers.at("PlayerSpawn").y);
        start_door.open();
    }
    // update camera
    Camera::tick(static_cast<int>(player->x));
    // remove any dead pigs
    for (int i = pigs.size() - 1; i >= 0; i--) {
        if (pigs[i]->should_be_deleted(*this)) {
            delete pigs[i];
            pigs.erase(pigs.begin() + i);
        }
    }
}

void Level::paint() {
    level.paint();
    start_door.paint();
    end_door.paint();
    player->paint();
    for (auto pig : pigs) { 
        pig->paint(); 
    }
    healthbar.paint(player->health);
}

void Level::send_event(LevelEvent event, Character& character) {
    player->handle_event(event, *this, character);
    for (auto pig : pigs) { 
        pig->handle_event(event, *this, character);
    }
    healthbar.handle_event(event, *this, character);
}