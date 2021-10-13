/*
Parse maps and tilesets from the Tiled map editor into game objects.
*/

#include <sstream>
#include <stdexcept>

#include "Camera.hpp"
#include "Assets.hpp"

#include "Tiles.hpp"

//--------------------------------------------
//            XML HELPER FUNCTIONS
//--------------------------------------------

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

//--------------------------------------------
//                 TILESET
//--------------------------------------------

Tileset::Tileset(int first_tile_id, std::string filename):
    sprite(create_sprite_from_tileset_file(filename)),
    first_tile_id(first_tile_id),
    last_tile_id(first_tile_id + sprite.get_num_frames() - 1)
{}

bool Tileset::contains_id(int id) {
    return first_tile_id <= id && id <= last_tile_id;
}

void Tileset::paint(int x, int y, int id) {
    sprite.paint(x, y, id - first_tile_id);
}

Sprite Tileset::create_sprite_from_tileset_file(std::string filename) {
    tinyxml2::XMLDocument doc;
    xml_assert(doc.LoadFile(filename.c_str()));

    auto tileset_node = doc.FirstChildElement();
    int tile_width = get_int_attribute(tileset_node, "tilewidth");
    int tile_height = get_int_attribute(tileset_node, "tileheight");

    auto image_node = tileset_node->FirstChildElement();
    std::string image_path = Assets::PATH + get_string_attribute(image_node, "source");
    int image_width = get_int_attribute(image_node, "width");
    int image_height = get_int_attribute(image_node, "height");

    return Sprite(image_path, image_width, image_height, tile_width, tile_height);
}

//--------------------------------------------
//                  TILEMAP
//--------------------------------------------

Tilemap::Tilemap(std::string filename) : 
    name(filename) 
{
    tinyxml2::XMLDocument doc;
    xml_assert(doc.LoadFile(filename.c_str()));

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

void Tilemap::paint() {
    paint_layer(layers["Background"]);
    paint_layer(layers["Midground"]);
}

void Tilemap::paint_layer(matrix& M) {
    for (size_t r = 0; r < M.size(); r++) {
        for (size_t c = 0; c < M[0].size(); c++) {
            if (M[r][c] == 0) continue; // 0 means there's no tile painted there.
            
            int tile_index = M[r][c];
            int level_x = c * tile_width;
            int level_y = r * tile_height;
            
            if (!Camera::is_box_visible(level_x, level_y, tile_width, tile_height)) continue;
            
            auto [screen_x, screen_y] = Camera::convert_to_screen_coordinates(level_x, level_y);
            auto it = find_if(tilesets.begin(), tilesets.end(), [tile_index](auto& t){ return t.contains_id(tile_index); });
            assert(it != tilesets.end());
            it->paint(screen_x, screen_y, tile_index);
        }
    }
}

void Tilemap::add_tileset(tinyxml2::XMLElement* node) {
    int first_id = get_int_attribute(node, "firstgid");
    std::string filename = Assets::PATH + get_string_attribute(node, "source");
    tilesets.emplace_back(first_id, filename);
}

void Tilemap::add_tile_layer(tinyxml2::XMLElement* node) {
    // get the name of the layer
    std::string layer_name = get_string_attribute(node, "name");
    // get the matrix of ints
    auto data_node = node->FirstChildElement();
    std::stringstream csv_text(std::string(data_node->GetText()));
    std::string temp;
    matrix M;
    while (csv_text >> temp) {
        M.push_back({});
        // split up the string of comma-separated ints
        std::stringstream s_stream(temp);
        std::string buffer;
        while (std::getline(s_stream, buffer, ',')) {
            M.back().push_back(std::stoi(buffer));
        }
    }
    layers[layer_name] = M;
}

void Tilemap::add_object_layer(tinyxml2::XMLElement* object_group_node) {
    std::string group_name = get_string_attribute(object_group_node, "name");
    if (group_name == "Collisions") {
        assert(collisions.empty());
        for (auto object_node = object_group_node->FirstChildElement(); object_node != nullptr; object_node = object_node->NextSiblingElement()) {
            SDL_Rect rect;
            rect.x = get_int_attribute(object_node, "x");
            rect.y = get_int_attribute(object_node, "y");
            rect.w = get_int_attribute(object_node, "width");
            rect.h = get_int_attribute(object_node, "height");
            collisions.push_back(rect);
        }
    } else if (group_name == "Markers") {
        for (auto object_node = object_group_node->FirstChildElement(); object_node != nullptr; object_node = object_node->NextSiblingElement()) {
            std::string object_name = get_string_attribute(object_node, "name");
            SDL_Rect rect;
            rect.x = get_int_attribute(object_node, "x");
            rect.y = get_int_attribute(object_node, "y");
            rect.w = get_int_attribute(object_node, "width");
            rect.h = get_int_attribute(object_node, "height");
            assert(!markers.count(object_name));
            markers[object_name] = rect;
        }
    } else {
        throw std::runtime_error("Unexpected name for objectgroup in tilemap file.");
    }
}