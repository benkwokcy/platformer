#pragma once

#include <tinyxml2.h>
#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>

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

void get_int_attribute(tinyxml2::XMLElement* element, const char* attribute_name, int* destination) {
    xml_assert(element->QueryIntAttribute(attribute_name, destination));
}

std::string get_string_attribute(tinyxml2::XMLElement* element, const char* attribute_name) {
    const char* temp;
    xml_assert(element->QueryStringAttribute(attribute_name, &temp));
    return std::string(temp);
}

/*********************************************
 *                  CLASSES
 *********************************************/

class Tileset {
public:
    std::string image_path;
    int first_id;
    int image_width, image_height;
    int tile_width, tile_height, tile_count, tile_columns;

    Tileset() {}

    Tileset(std::string filename, int first_id) :
        first_id(first_id)
    {
        tinyxml2::XMLDocument doc;
        xml_assert(doc.LoadFile((Assets::path + filename).c_str()));

        auto tileset_node = doc.FirstChildElement();
        get_int_attribute(tileset_node, "tilewidth", &tile_width);
        get_int_attribute(tileset_node, "tileheight", &tile_height);
        get_int_attribute(tileset_node, "tilecount", &tile_count);
        get_int_attribute(tileset_node, "columns", &tile_columns);
        
        auto image_node = tileset_node->FirstChildElement();
        image_path = get_string_attribute(image_node, "source");
        get_int_attribute(image_node, "width", &image_width);
        get_int_attribute(image_node, "height", &image_height);
    }
};

class Tilemap {
public:
    using matrix = std::vector<std::vector<int>>;

    int map_width, map_height;
    int tile_width, tile_height;
    std::vector<Tileset> tilesets;
    std::vector<matrix> layers;
    std::vector<SDL_Rect> collisions;

    Tilemap(std::string filename) {
        tinyxml2::XMLDocument doc;
        xml_assert(doc.LoadFile((Assets::path + filename).c_str()));

        auto map_node = doc.FirstChildElement();
        get_int_attribute(map_node, "width", &map_width);
        get_int_attribute(map_node, "height", &map_height);
        get_int_attribute(map_node, "tilewidth", &tile_width);
        get_int_attribute(map_node, "tileheight", &tile_height);
        
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

private:
    void read_tileset(tinyxml2::XMLElement* node) {
        int first_id;
        get_int_attribute(node, "firstgid", &first_id);
        tilesets.emplace_back(get_string_attribute(node, "source"), first_id);
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
            get_int_attribute(object_node, "x", &rect.x);
            get_int_attribute(object_node, "y", &rect.y);
            get_int_attribute(object_node, "width", &rect.w);
            get_int_attribute(object_node, "height", &rect.h);
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