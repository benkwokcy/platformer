#pragma once

#include <tinyxml2.h>
#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>

// Split a comma-separated string of ints into a vector of ints.
std::vector<int> split(const std::string& text) {
    std::vector<int> V;
    std::stringstream s_stream(text);
    std::string buffer;
    while (std::getline(s_stream, buffer, ',')) {
        V.push_back(std::stoi(buffer));
    }
    return V;
}

class Tileset {
public:
    std::string image_path;
    int image_width, image_height;
    int tile_width, tile_height, tile_count, tile_columns;

    Tileset() {}

    Tileset(std::string filename) {
        tinyxml2::XMLDocument doc;
        assert(doc.LoadFile(("assets/" + filename).c_str()) == tinyxml2::XML_SUCCESS);

        auto tileset_node = doc.FirstChildElement();
        assert(tileset_node->QueryIntAttribute("tilewidth", &tile_width) == tinyxml2::XML_SUCCESS);
        assert(tileset_node->QueryIntAttribute("tileheight", &tile_height) == tinyxml2::XML_SUCCESS);
        assert(tileset_node->QueryIntAttribute("tilecount", &tile_count) == tinyxml2::XML_SUCCESS);
        assert(tileset_node->QueryIntAttribute("columns", &tile_columns) == tinyxml2::XML_SUCCESS);
        
        auto image_node = tileset_node->FirstChildElement();
        const char* temp;
        assert(image_node->QueryStringAttribute("source", &temp) == tinyxml2::XML_SUCCESS);
        image_path = temp;
        assert(image_node->QueryIntAttribute("width", &image_width) == tinyxml2::XML_SUCCESS);
        assert(image_node->QueryIntAttribute("height", &image_height) == tinyxml2::XML_SUCCESS);
    }
};

class Tilemap {
public:
    int map_width, map_height;
    int tile_width, tile_height;
    Tileset tileset;
    std::vector<std::vector<int>> tile_layer;
    std::vector<SDL_Rect> collisions;

    Tilemap(std::string filename) {
        tinyxml2::XMLDocument doc;
        assert(doc.LoadFile(("assets/" + filename).c_str()) == tinyxml2::XML_SUCCESS);

        auto map_node = doc.FirstChildElement();
        assert(map_node->QueryIntAttribute("width", &map_width) == tinyxml2::XML_SUCCESS);
        assert(map_node->QueryIntAttribute("height", &map_height) == tinyxml2::XML_SUCCESS);
        assert(map_node->QueryIntAttribute("tilewidth", &tile_width) == tinyxml2::XML_SUCCESS);
        assert(map_node->QueryIntAttribute("tileheight", &tile_height) == tinyxml2::XML_SUCCESS);
        
        auto curr = map_node->FirstChildElement();
        while (curr) {
            std::string type(curr->Value());
            if (type == "tileset") {
                const char* temp;
                assert(curr->QueryStringAttribute("source", &temp) == tinyxml2::XML_SUCCESS);
                tileset = Tileset(temp);
            } else if (type == "layer") {
                auto data_node = curr->FirstChildElement();
                std::string raw_data(data_node->GetText());
                std::stringstream data_stream(raw_data);
                std::string temp;
                std::vector<std::string> rows;
                while (data_stream >> temp) {
                    tile_layer.push_back(split(temp));
                }
            } else if (type == "objectgroup") {
                auto object_node = curr->FirstChildElement();
                while (object_node) {
                    SDL_Rect o;
                    assert(object_node->QueryIntAttribute("x", &o.x) == tinyxml2::XML_SUCCESS);
                    assert(object_node->QueryIntAttribute("y", &o.y) == tinyxml2::XML_SUCCESS);
                    assert(object_node->QueryIntAttribute("width", &o.w) == tinyxml2::XML_SUCCESS);
                    assert(object_node->QueryIntAttribute("height", &o.h) == tinyxml2::XML_SUCCESS);
                    collisions.push_back(o);
                    object_node = object_node->NextSiblingElement();
                }
            } else {
                throw std::runtime_error("Unexpected node in Tilemap.");
            }
            curr = curr->NextSiblingElement();
        }
    }
};