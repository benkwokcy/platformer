#pragma once

#include <utility>
#include <iostream>

#include "Window.hpp"

// Side scrolling camera
class Camera {
public:
    static inline int x = 0, y = 0; // top left corner of the camera view in level coordinates, this position is directly modified by the level

    static void tick(int player_x, int player_y) {
        int left_boundary = x + (0.3 * Window::width);
        int right_boundary = x + (0.7 * Window::width);
        if (player_x > right_boundary) {
            x += player_x - right_boundary;
        } else if (player_x < left_boundary) {
            x += player_x - left_boundary;
        }

        // TODO - camera shouldn't fall below the fall death marker, or out of the level boundaries in general
        int top_boundary = y + (0.2 * Window::height);
        int bottom_boundary = y + (0.8 * Window::height);
        if (player_y < top_boundary) {
            y += player_y - top_boundary;
        } else if (player_y > bottom_boundary) {
            y += player_y - bottom_boundary;
        }
    }

    // Returns true if the given box is visible from the camera's POV.
    static bool is_box_visible(int level_x, int level_y, int width, int height) {
        return level_x + width >= x && level_y + height >= y && level_x < x + Window::width && level_y < y + Window::height;
    }

    static std::pair<int,int> convert_to_screen_coordinates(int level_x, int level_y) {
        return { level_x - x, level_y - y };
    }
};