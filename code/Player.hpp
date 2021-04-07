#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Window.hpp"

class Player : public Entity {
public:
    Player() {
        // image = IMG_Load("assets/sprites/playerrun.png");
        // assert(image != nullptr);
        // texture = SDL_CreateTextureFromSurface(Window::renderer, image);
    }

    ~Player() {
        // SDL_DestroyTexture(texture);
        // SDL_FreeSurface(image);
    }

    void paint() const override {
        // SDL_RenderCopy(Window::renderer, texture, NULL, NULL);
    }

    void handle_event(Event e) override {}
    void tick() override {}
private:
    // SDL_Surface* image;
    // SDL_Texture* texture;
};