#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Window.hpp"

class Sprite {
public:
    int num_frames;

    Sprite() {}

    // The sprite will be centered at position (x,y)
    Sprite(const char* filename, int frame_width, int frame_height, int num_frames = 1, int frames_per_second = -1) : 
        frame_width(frame_width),
        frame_height(frame_height),
        num_frames(num_frames),
        frames_per_second(frames_per_second),
        creation_time(static_cast<int>(SDL_GetTicks()))
    {
        if (surface = IMG_Load(filename); surface == nullptr) {
            throw runtime_error("Failed IMG_Load.");
        }
        if (texture = SDL_CreateTextureFromSurface(Window::renderer, surface); texture == nullptr) {
            throw runtime_error("Failed CreateTextureFromSurface.");
        }

        source_rect.x = 0;
        source_rect.y = 0;
        source_rect.w = frame_width;
        source_rect.h = frame_height;
        dest_rect.w = frame_width;
        dest_rect.h = frame_height;
    }

    ~Sprite() {
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    Sprite(const Sprite& other) = delete;
    Sprite& operator=(const Sprite& other) = delete;

    void paint(int x = Window::center_x(), int y = Window::center_y(), bool flip = false) {
        // update position
        dest_rect.x = x - frame_width / 2;
        dest_rect.y = y - frame_height / 2;
        // update frame
        if (num_frames > 1) {
            int frame_index = ((SDL_GetTicks() - creation_time) * frames_per_second / 1000) % num_frames;
            source_rect.x = frame_width * frame_index;
        }
        if (flip) {
            SDL_RenderCopyEx(Window::renderer, texture, &source_rect, &dest_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopy(Window::renderer, texture, &source_rect, &dest_rect);
        }
    }

    int frames_elapsed() {
        return (SDL_GetTicks() - creation_time) * frames_per_second / 1000;
    }

    void set_first_frame() {
        creation_time = SDL_GetTicks();
    }

private:
    int frame_width;
    int frame_height;
    int frames_per_second;
    int creation_time;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect source_rect;
    SDL_Rect dest_rect;
};