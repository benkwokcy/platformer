#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Window.hpp"

class Sprite {
public:
    SDL_Rect dest_rect;

    Sprite(const char* filename, int x = Window::center_x(), int y = Window::center_y()) {
        if (surface = IMG_Load(filename); surface == nullptr) {
            throw runtime_error("Failed IMG_Load.");
        }
        if (texture = SDL_CreateTextureFromSurface(Window::renderer, surface); texture == nullptr) {
            throw runtime_error("Failed CreateTextureFromSurface.");
        }

        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        dest_rect.x = x - width / 2;
        dest_rect.y = y - height / 2;
        dest_rect.w = width;
        dest_rect.h = height;
    }

    ~Sprite() {
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    Sprite(const Sprite& other) = delete;
    Sprite& operator=(const Sprite& other) = delete;

    void paint() const {
        SDL_RenderCopy(Window::renderer, texture, NULL, &dest_rect);
    }

    void next_frame() {
        // TODO
    }
private:
    SDL_Surface* surface;
    SDL_Texture* texture;
};