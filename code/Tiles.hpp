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

#include "Camera.hpp"
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
 *               TILE CLASSES
 *********************************************/

class Tileset {
public:
    Tileset(int first_tile_id, std::string image_path, int image_width, int image_height, int tile_width, int tile_height):
        sprite(std::make_unique<Sprite>(image_path, image_width, image_height, tile_width, tile_height)),
        first_tile_id(first_tile_id),
        last_tile_id(first_tile_id + sprite->get_num_frames() - 1)
    {}

    bool contains_id(int id) {
        return first_tile_id <= id && id <= last_tile_id;
    }

    void paint(int x, int y, int id) {
        sprite->paint(x, y, id - first_tile_id);
    }

private:
    std::unique_ptr<Sprite> sprite; // we use Sprite pointers because Sprites are not copyable or moveable.
    int first_tile_id;
    int last_tile_id;
};

class Tilemap {
public:
    using matrix = std::vector<std::vector<int>>;
    using first_tile_id = int;

    std::string name;
    int map_width, map_height;
    int tile_width, tile_height;
    std::vector<Tileset> tilesets;
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

    // TODO - should I just have 3 members - Background, Midground, Foreground? Or will I have other layers?
    void paint() {
        paint_layer(layers["Background"]);
        paint_layer(layers["Midground"]);
    }

private:
    // TODO - don't hardcode to [0,0] screen coordinates
    void paint_layer(matrix& M) {
        for (size_t r = 0; r < M.size(); r++) {
            for (size_t c = 0; c < M[0].size(); c++) {
                if (M[r][c] != 0) {
                    int level_x = c * tile_width;
                    int level_y = r * tile_height;
                    if (Camera::is_visible(level_x, level_y, tile_width, tile_height)) {
                        auto [screen_x, screen_y] = Camera::convert_level_to_screen_coordinates(level_x, level_y);
                        paint_tile(screen_x, screen_y, M[r][c]);
                    }
                }
            }
        }
    }

    void paint_tile(int screen_x, int screen_y, int tile_index) {
        auto it = find_if(tilesets.begin(), tilesets.end(), [tile_index](auto& t){ return t.contains_id(tile_index); });
        assert(it != tilesets.end());
        it->paint(screen_x, screen_y, tile_index);
    }

    void add_tileset(tinyxml2::XMLElement* node) {
        int first_id = get_int_attribute(node, "firstgid");
        std::string filename = get_string_attribute(node, "source");

        tinyxml2::XMLDocument doc;
        xml_assert(doc.LoadFile((Assets::path + filename).c_str()));

        auto tileset_node = doc.FirstChildElement();
        int tile_width_ = get_int_attribute(tileset_node, "tilewidth");
        int tile_height_ = get_int_attribute(tileset_node, "tileheight");

        auto image_node = tileset_node->FirstChildElement();
        std::string image_path = Assets::path + get_string_attribute(image_node, "source");
        int image_width = get_int_attribute(image_node, "width");
        int image_height = get_int_attribute(image_node, "height");

        tilesets.emplace_back(first_id, image_path, image_width, image_height, tile_width_, tile_height_);
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