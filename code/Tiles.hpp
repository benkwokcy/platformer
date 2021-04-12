/*
Parse maps and tilesets from the Tiled map editor into game objects.
*/

#pragma once

#include <SDL2/SDL.h>
#include <tinyxml2.h>

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

#include "Sprite.hpp"

/*********************************************
 *               CONSTANTS
 *********************************************/

namespace Assets {
    std::string path("assets/");
};

/*********************************************
 *            XML HELPER FUNCTIONS
 *********************************************/

inline void xml_assert(tinyxml2::XMLError error_code) {
    assert(error_code == tinyxml2::XML_SUCCESS);
}

int get_int_attribute(tinyxml2::XMLElement* element, const char* attribute_name) {
    int temp;
    xml_assert(element->QueryIntAttribute(attribute_name, &temp));
    return temp;
}

std::string get_string_attribute(tinyxml2::XMLElement* element, const char* attribute_name) {
    const char* temp;
    xml_assert(element->QueryStringAttribute(attribute_name, &temp));
    return std::string(temp);
}

/*********************************************
 *                 TILE CODE
 *********************************************/

std::unique_ptr<Sprite> create_sprite_from_tileset(std::string filename) {
    std::string image_path;
    int image_width, image_height;
    int tile_width, tile_height;

    tinyxml2::XMLDocument doc;
    xml_assert(doc.LoadFile((Assets::path + filename).c_str()));

    auto tileset_node = doc.FirstChildElement();
    tile_width = get_int_attribute(tileset_node, "tilewidth");
    tile_height = get_int_attribute(tileset_node, "tileheight");

    auto image_node = tileset_node->FirstChildElement();
    image_path = Assets::path + get_string_attribute(image_node, "source");
    image_width = get_int_attribute(image_node, "width");
    image_height = get_int_attribute(image_node, "height");

    return std::make_unique<Sprite>(image_path, image_width, image_height, tile_width, tile_height);
}

class Tilemap {
public:
    using matrix = std::vector<std::vector<int>>;
    using first_tile_id = int;

    std::string name;
    int map_width, map_height;
    int tile_width, tile_height;
    std::map<first_tile_id, std::unique_ptr<Sprite>> tilesets; // we use Sprite pointers because Sprites are not copyable or moveable.
    std::unordered_map<std::string,matrix> layers; // 2D grids of tile indices
    std::vector<SDL_Rect> collisions; // the bounding boxes to collide against

    Tilemap(std::string filename) : 
        name(filename) 
    {
        tinyxml2::XMLDocument doc;
        xml_assert(doc.LoadFile((Assets::path + filename).c_str()));

        auto map_node = doc.FirstChildElement();
        map_width = get_int_attribute(map_node, "width");
        map_height = get_int_attribute(map_node, "height");
        tile_width = get_int_attribute(map_node, "tilewidth");
        tile_height = get_int_attribute(map_node, "tileheight");

        for (auto curr = map_node->FirstChildElement(); curr != nullptr; curr = curr->NextSiblingElement()) {
            std::string type(curr->Value());
            if (type == "tileset") {
                add_tileset(curr);
            } else if (type == "layer") {
                add_tile_layer(curr);
            } else if (type == "objectgroup") {
                add_object_layer(curr);
            } else {
                throw std::runtime_error("Unexpected node in Tilemap.");
            }
        }
    }

    // TODO - don't hardcode to [0,0] screen coordinates
    void paint() {
        paint_layer(layers["Background"]);
        paint_layer(layers["Midground"]);
    }

private:
    void paint_layer(matrix& M) {
        for (size_t r = 0; r < M.size(); r++) {
            for (size_t c = 0; c < M[0].size(); c++) {
                if (M[r][c] != 0) {
                    paint_tile(c * tile_width, r * tile_height, M[r][c]);
                }
            }
        }
    }

    // TODO - Since we have so few tilemaps, it's faster to use a vector
    void paint_tile(int x, int y, int tile_index) {
        auto& [first_tile_id, sprite_ptr] = *(--tilesets.upper_bound(tile_index));
        sprite_ptr->paint(x, y, tile_index - first_tile_id);
    }

    void add_tileset(tinyxml2::XMLElement* node) {
        int first_id;
        first_id = get_int_attribute(node, "firstgid");
        auto filename = get_string_attribute(node, "source");
        tilesets.emplace(std::make_pair(first_id, create_sprite_from_tileset(filename)));
    }

    // TODO - get more layer info
    void add_tile_layer(tinyxml2::XMLElement* node) {
        std::string layer_name = get_string_attribute(node, "name");
        auto data_node = node->FirstChildElement();
        std::stringstream csv_text(std::string(data_node->GetText()));
        std::string temp;
        matrix M;
        while (csv_text >> temp) {
            M.push_back(split_comma_separated_ints(temp));
        }
        layers[layer_name] = M;
    }

    // TODO - handle polygons and circles
    void add_object_layer(tinyxml2::XMLElement* object_group_node) {
        for (auto object_node = object_group_node->FirstChildElement(); object_node != nullptr; object_node = object_node->NextSiblingElement()) {
            SDL_Rect rect;
            rect.x = get_int_attribute(object_node, "x");
            rect.y = get_int_attribute(object_node, "y");
            rect.w = get_int_attribute(object_node, "width");
            rect.h = get_int_attribute(object_node, "height");
            collisions.push_back(rect);
        }
    }

    std::vector<int> split_comma_separated_ints(const std::string& text) {
        std::vector<int> V;
        std::stringstream s_stream(text);
        std::string buffer;
        while (std::getline(s_stream, buffer, ',')) {
            V.push_back(std::stoi(buffer));
        }
        return V;
    }
};