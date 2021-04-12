#pragma once

#include <tinyxml2.h>
#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <utility>

#include "Sprite.hpp"

/*********************************************
 *               CONSTANTS
 *********************************************/

namespace Assets {
    std::string path("assets/");
};

/*********************************************
 *              XML HELPER FUNCTIONS
 *********************************************/

void xml_assert(tinyxml2::XMLError error_code) {
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
 *                  MAIN CODE
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

    return std::make_unique<Sprite>(image_path, image_width, image_height, tile_width, tile_height, 0, 0);
}

class Tilemap {
public:
    using matrix = std::vector<std::vector<int>>;

    std::string name;
    int map_width, map_height;
    int tile_width, tile_height; // is this needed?
    std::map<int, std::unique_ptr<Sprite>> tilesets; // firstId : unique Sprite pointer
    std::vector<matrix> layers;
    std::vector<SDL_Rect> collisions;

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
                read_tileset(curr);
            } else if (type == "layer") {
                read_tile_layer(curr);
            } else if (type == "objectgroup") {
                read_object_layer(curr);
            } else {
                throw std::runtime_error("Unexpected node in Tilemap.");
            }
        }
    }

    // TODO - paint

private:

    void read_tileset(tinyxml2::XMLElement* node) {
        int first_id;
        first_id = get_int_attribute(node, "firstgid");
        auto filename = get_string_attribute(node, "source");
        tilesets.emplace(std::make_pair(first_id, create_sprite_from_tileset(filename)));
    }

    // TODO - get more layer info
    void read_tile_layer(tinyxml2::XMLElement* node) {
        auto data_node = node->FirstChildElement();
        std::stringstream csv_text(std::string(data_node->GetText()));
        std::string temp;
        matrix M;
        while (csv_text >> temp) {
            M.push_back(split_comma_separated_ints(temp));
        }
        layers.push_back(M);
    }

    // TODO - handle polygons and circles
    void read_object_layer(tinyxml2::XMLElement* object_group_node) {
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