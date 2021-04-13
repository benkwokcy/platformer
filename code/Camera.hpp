#pragma once

#include <utility>
#include <iostream>

#include "Window.hpp"

// Side scrolling camera
class Camera {
public:
    static inline int x = 0, y = 0; // top left corner of the camera view in level coordinates

    // Called by the player every frame
    static void tick(int player_x) {
        int left_boundary = x + (0.3 * Window::width);
        int right_boundary = x + (0.7 * Window::width);
        if (player_x > right_boundary) {
            x += player_x - right_boundary;
        } else if (player_x < left_boundary) {
            x += player_x - left_boundary;
        }
    }

    // Returns true if the given box is within the camera's view
    static bool is_visible(int level_x, int level_y, int width, int height) {
        return level_x + width >= x && level_y + height >= y && level_x < x + Window::width && level_y < y + Window::height;
    }

    static std::pair<int,int> convert_to_screen_coordinates(int level_x, int level_y) {
        return { level_x - x, level_y - y };
    }
};