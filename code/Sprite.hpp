#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Window.hpp"

class Sprite {
public:
    Sprite(const char* filename, int frame_width, int frame_height, SDL_Rect bounding_box, int num_frames = 1, int frames_per_second = -1) : 
        frame_width(frame_width),
        frame_height(frame_height),
        bounding_box(bounding_box),
        num_frames(num_frames),
        frames_per_second(frames_per_second),
        creation_time(static_cast<int>(SDL_GetTicks()))
    {
        if (surface = IMG_Load(filename); surface == nullptr) {
            throw std::runtime_error("Failed IMG_Load.");
        }
        if (texture = SDL_CreateTextureFromSurface(Window::renderer, surface); texture == nullptr) {
            throw std::runtime_error("Failed CreateTextureFromSurface.");
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

    void paint(int x = 0, int y = 0, bool flip = false) {
        // update position
        dest_rect.x = x - bounding_box.x;
        dest_rect.y = y - bounding_box.y;
        // update frame
        if (num_frames > 1) {
            int frame_index = frames_elapsed() % num_frames;
            source_rect.x = frame_width * frame_index;
        }
        if (flip) {
            SDL_RenderCopyEx(Window::renderer, texture, &source_rect, &dest_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopy(Window::renderer, texture, &source_rect, &dest_rect);
        }
    }

    void set_first_frame() {
        creation_time = SDL_GetTicks();
    }

    int frames_elapsed() {
        return ((SDL_GetTicks() - creation_time) * frames_per_second / 1000);
    }

    int get_num_frames() {
        return num_frames;
    }

private:
    int frame_width;
    int frame_height;
    int num_frames;
    int frames_per_second;
    int creation_time;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect source_rect;
    SDL_Rect dest_rect;
    SDL_Rect bounding_box;
};