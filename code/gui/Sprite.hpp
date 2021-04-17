#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

#include "Window.hpp"

/*********************************************
 *                 SPRITE
 *********************************************/

class Sprite {
public:
    // Convenience constructor for a normal image with no frames
    Sprite(std::string filename, int image_width, int image_height)
        : Sprite(filename, image_width, image_height, image_width, image_height, image_width, image_height, 0, 0)
    {}

    // Convenience constructor for images with multiple frames and no offset
    Sprite(std::string filename, int image_width, int image_height, int frame_width, int frame_height)
        : Sprite(filename, image_width, image_height, frame_width, frame_height, frame_width, frame_height, 0, 0)
    {}

    // Main constructor
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

    // Sprites cannot be copied
    Sprite(const Sprite& other) = delete;
    Sprite& operator=(const Sprite& other) = delete;

    Sprite(Sprite&& other) {
        filename = std::move(other.filename);
        image_width = other.image_width;
        image_height = other.image_height;
        frame_width = other.frame_width;
        frame_height = other.frame_height;
        sprite_width = other.sprite_width;
        sprite_height = other.sprite_height;
        x_offset = other.x_offset;
        y_offset = other.y_offset;
        num_rows = other.num_rows;
        num_cols = other.num_cols;
        num_frames = other.num_frames;
        surface = other.surface; other.surface = nullptr;
        texture = other.texture; other.texture = nullptr;
        source_rect = other.source_rect;
        dest_rect = other.dest_rect;
    }

    // I could implement this but not necessary right now
    Sprite& operator=(Sprite&& other) = delete;

    int get_num_frames() {
        return num_frames;
    }

    void paint(int screen_x = 0, int screen_y = 0, int index = 0, bool horizontal_flip = false) {
        source_rect.x = (index % num_cols) * frame_width;
        source_rect.y = (index / num_cols) * frame_height;
        if (horizontal_flip) {
            dest_rect.x = screen_x - (frame_width - x_offset - sprite_width);
            dest_rect.y = screen_y - y_offset;
            SDL_RenderCopyEx(Window::renderer, texture, &source_rect, &dest_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            dest_rect.x = screen_x - x_offset;
            dest_rect.y = screen_y - y_offset;
            SDL_RenderCopy(Window::renderer, texture, &source_rect, &dest_rect);
        }
    }

protected:
    std::string filename;
    int image_width, image_height;
    int frame_width, frame_height;
    int sprite_width, sprite_height; // if the sprite is smaller than the frame
    int x_offset, y_offset; // if the sprite is smaller than the frame
    int num_rows, num_cols, num_frames;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect source_rect; // image coordinates
    SDL_Rect dest_rect; // screen coordinates
};

/*********************************************
 *            ANIMATED SPRITE
 *********************************************/

// Doesn't necessarily need to have more than one frame.
class AnimatedSprite : public Sprite {
public:
    AnimatedSprite(const char* filename, int image_width, int image_height, int frame_width, int frame_height, int sprite_width, int sprite_height, 
                   int x_offset, int y_offset, int frames_per_second) : 
        Sprite(filename, image_width, image_height, frame_width, frame_height, sprite_width, sprite_height, x_offset, y_offset),
        frames_per_second(frames_per_second)
    {}

    AnimatedSprite(const AnimatedSprite& other) = delete;
    AnimatedSprite& operator=(const AnimatedSprite& other) = delete;

    AnimatedSprite(AnimatedSprite&& other) :
        Sprite(std::move(other))
    {
        frames_per_second = other.frames_per_second;
    }

    AnimatedSprite& operator=(AnimatedSprite&& other) = delete;

    void paint(int screen_x, int screen_y, int creation_time, bool horizontal_flip = false) {
        int frame_index = frames_elapsed(creation_time) % num_frames;
        Sprite::paint(screen_x, screen_y, frame_index, horizontal_flip);
    }

    // Returns true if the animation has completed one loop
    bool animation_complete(int creation_time) {
        return frames_elapsed(creation_time) >= num_frames;
    }

private:
    int frames_per_second;

    // Get the number of frames elapsed since creation or the last reset
    int frames_elapsed(int creation_time) {
        return (SDL_GetTicks() - creation_time) * frames_per_second / 1000;
    }
};