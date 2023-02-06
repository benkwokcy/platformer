#pragma once

#include <tinyxml2.h>

#include "Sprite.hpp"

#include <unordered_map>
#include <vector>
#include <string>

/*
Parse maps and tilesets from the Tiled map editor into game objects.
*/

class Tileset {
public:
    Tileset(int first_tile_id, std::string filename);
    bool contains_id(int id);
    void paint(int x, int y, int id);

private:
    Sprite sprite;
    int first_tile_id;
    int last_tile_id;

    Sprite create_sprite_from_tileset_file(std::string filename);
};

class Tilemap {
public:
    using matrix = std::vector<std::vector<int>>;

    std::string name;
    int map_width, map_height;
    int tile_width, tile_height;
    std::vector<Tileset> tilesets;
    std::unordered_map<std::string, matrix> layers; // 2D grids of tile indices
    std::vector<SDL_Rect> collisions; // bounding boxes to collide against
    std::unordered_map<std::string, SDL_Rect> markers; // ex. spawn points, AI paths

    Tilemap(std::string filename);
    void paint();

private:
    void paint_layer(matrix& M);
    void add_tileset(tinyxml2::XMLElement* node);
    void add_tile_layer(tinyxml2::XMLElement* node);
    void add_object_layer(tinyxml2::XMLElement* object_group_node);
};