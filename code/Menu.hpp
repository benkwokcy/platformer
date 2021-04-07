#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Window.hpp"

// The main menu.
class Menu : public Entity {
public:
    Menu() {
        if (image = IMG_Load("sprites/mainmenu.png"); image == nullptr) {
            throw runtime_error("Failed IMG_Load for Menu.");
        }
        if (texture = SDL_CreateTextureFromSurface(Window::renderer, image); texture == nullptr) {
            throw runtime_error("Failed CreateTextureFromSurface for Menu.");
        }
    }

    ~Menu() {
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(image);
    }

    void paint() const override {
        SDL_RenderCopy(Window::renderer, texture, NULL, NULL);
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    SDL_Surface* image;
    SDL_Texture* texture;
};