#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Window.hpp"

class Sprite {
public:
    Sprite(std::string filename, int image_width, int image_height, int frame_width, int frame_height, int sprite_width, int sprite_height, int x_offset, int y_offset) : 
        filename(filename),
        image_width(image_width),
        image_height(image_height),
        frame_width(frame_width),
        frame_height(frame_height),
        sprite_width(sprite_width),
        sprite_height(sprite_height),
        x_offset(x_offset),
        y_offset(y_offset)
    {
        if (surface = IMG_Load(filename.c_str()); surface == nullptr) {
            throw std::runtime_error("Failed IMG_Load.");
        }
        if (texture = SDL_CreateTextureFromSurface(Window::renderer, surface); texture == nullptr) {
            throw std::runtime_error("Failed CreateTextureFromSurface.");
        }

        num_cols = image_width / frame_width;
        num_rows = image_height / frame_height;
        num_frames = num_cols * num_rows;
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
    Sprite(Sprite&& other) = delete;
    Sprite& operator=(Sprite&& other) = delete;

    void paint(int x = 0, int y = 0, int index = 0, bool horizontal_flip = false) {
        source_rect.x = (index % num_cols) * frame_width;
        source_rect.y = (index / num_cols) * frame_height;
        if (horizontal_flip) {
            dest_rect.x = x - (frame_width - x_offset - sprite_width);
            dest_rect.y = y - y_offset;
            SDL_RenderCopyEx(Window::renderer, texture, &source_rect, &dest_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            dest_rect.x = x - x_offset;
            dest_rect.y = y - y_offset;
            SDL_RenderCopy(Window::renderer, texture, &source_rect, &dest_rect);
        }
    }

protected:
    std::string filename;
    int image_width, image_height;
    int frame_width, frame_height;
    int sprite_width, sprite_height; // if the sprite is smaller than the frame
    int num_rows, num_cols, num_frames;
    int x_offset, y_offset; // if the sprite is smaller than the frame
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect source_rect;
    SDL_Rect dest_rect;
};

// Doesn't necessarily need to have more than one frame.
class AnimatedSprite : public Sprite {
public:
    AnimatedSprite(const char* filename, int image_width, int image_height, int frame_width, int frame_height, int sprite_width, int sprite_height, 
                   int x_offset, int y_offset, int frames_per_second) : 
        Sprite(filename, image_width, image_height, frame_width, frame_height, sprite_width, sprite_height, x_offset, y_offset),
        frames_per_second(frames_per_second),
        creation_time(static_cast<int>(SDL_GetTicks()))
    {}

    AnimatedSprite(const AnimatedSprite& other) = delete;
    AnimatedSprite& operator=(const AnimatedSprite& other) = delete;
    AnimatedSprite(AnimatedSprite&& other) = delete;
    AnimatedSprite& operator=(AnimatedSprite&& other) = delete;

    void paint(int x = 0, int y = 0, bool horizontal_flip = false) {
        int frame_index = frames_elapsed() % num_frames;
        Sprite::paint(x, y, frame_index, horizontal_flip);
    }

    // Reset the sprite to the first frame
    void set_first_frame() {
        creation_time = SDL_GetTicks();
    }

    // Returns true if the animation has completed one loop
    bool animation_complete() {
        return frames_elapsed() >= num_frames;
    }

private:
    int frames_per_second;
    int creation_time;

    // Get the number of frames elapsed since creation or the last reset
    int frames_elapsed() {
        return (SDL_GetTicks() - creation_time) * frames_per_second / 1000;
    }
};