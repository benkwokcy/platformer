#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <unordered_map>

#include "Window.hpp"

/*********************************************
 *                 SPRITE
 *********************************************/

class Sprite {
public:
    // Convenience constructor for a normal image with no frames
    Sprite(std::string filename, int image_width, int image_height)
        : Sprite(filename, image_width, image_height, image_width, image_height, image_width, image_height)
    {}

    // Convenience constructor for images with multiple frames and no offset
    Sprite(std::string filename, int image_width, int image_height, int frame_width, int frame_height)
        : Sprite(filename, image_width, image_height, frame_width, frame_height, frame_width, frame_height)
    {}

    // Main constructor
    Sprite(std::string filename, int image_width, int image_height, int frame_width, int frame_height, int sprite_width, int sprite_height, 
           int x_offset = 0, int y_offset = 0, bool faces_left = false) : 
        image_width(image_width),
        image_height(image_height),
        frame_width(frame_width),
        frame_height(frame_height),
        sprite_width(sprite_width),
        sprite_height(sprite_height),
        x_offset(x_offset),
        y_offset(y_offset),
        faces_left(faces_left)
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

    Sprite(Sprite&& other) : 
        image_width(other.image_width),
        image_height(other.image_height),
        frame_width(other.frame_width),
        frame_height(other.frame_height),
        sprite_width(other.sprite_width),
        sprite_height(other.sprite_height),
        x_offset(other.x_offset),
        y_offset(other.y_offset),
        num_rows(other.num_rows),
        num_cols(other.num_cols),
        num_frames(other.num_frames),
        faces_left(other.faces_left),
        surface(std::exchange(other.surface, nullptr)),
        texture(std::exchange(other.texture, nullptr)),
        source_rect(other.source_rect),
        dest_rect(other.dest_rect)
    {}

    // I could implement this but not necessary right now
    Sprite& operator=(Sprite&& other) = delete;

    int get_num_frames() {
        return num_frames;
    }

    void paint(int screen_x = 0, int screen_y = 0, int index = 0, bool horizontal_flip = false) {
        if (faces_left) { horizontal_flip = !horizontal_flip; }
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
    int image_width, image_height;
    int frame_width, frame_height;
    int sprite_width, sprite_height; // if the sprite is smaller than the frame
    int x_offset, y_offset; // if the sprite is smaller than the frame
    int num_rows, num_cols, num_frames;
    bool faces_left; // true if the sprite is drawn facing left
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
                   int x_offset, int y_offset, int frames_per_second, bool faces_left = false) : 
        Sprite(filename, image_width, image_height, frame_width, frame_height, sprite_width, sprite_height, x_offset, y_offset, faces_left),
        frames_per_second(frames_per_second),
        creation_time(static_cast<int>(SDL_GetTicks()))
    {}

    AnimatedSprite(const AnimatedSprite& other) = delete;
    AnimatedSprite& operator=(const AnimatedSprite& other) = delete;

    AnimatedSprite(AnimatedSprite&& other) :
        Sprite(std::move(other))
    {
        frames_per_second = other.frames_per_second;
        creation_time = other.creation_time;
        collisions = std::move(other.collisions);
    }

    AnimatedSprite& operator=(AnimatedSprite&& other) = delete;

    void paint(int screen_x, int screen_y, bool horizontal_flip = false) {
        Sprite::paint(screen_x, screen_y, get_frame_index(), horizontal_flip);
    }

    // TODO - Show bounding boxes
    // void paint_debug(int screen_x, int screen_y, bool horizontal_flip = false) {
    //     if (has_collision) {

    //     }
    //     Sprite::paint(screen_x, screen_y, get_frame_index(), horizontal_flip);
    // }

    int get_frame_index() {
        return frames_elapsed() % num_frames;
    }

    // Returns true if the animation has completed one loop
    bool animation_complete() {
        return frames_elapsed() >= num_frames;
    }

    // Reset the animation start time.
    void reset_time() {
        creation_time = SDL_GetTicks();
    }

    void add_collision(int index, SDL_Rect rect) {
        assert(collisions.count(index) == 0);
        collisions[index] = rect;
    }

    bool has_collision() {
        return collisions.count(get_frame_index()) != 0;
    }

    // Convert frame coordinates to level coordinates
    SDL_Rect get_collision(int level_x, int level_y) {
        SDL_Rect collision = collisions.at(get_frame_index());
        collision.x = level_x - (x_offset - collision.x);
        collision.y = level_y - (y_offset - collision.y);
        return collision;
    }

private:
    int frames_per_second;
    int creation_time;
    std::unordered_map<int,SDL_Rect> collisions;

    // Get the number of frames elapsed since creation or the last reset
    int frames_elapsed() {
        return (SDL_GetTicks() - creation_time) * frames_per_second / 1000;
    }
};