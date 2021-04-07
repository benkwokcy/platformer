#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Scene.hpp"
#include "Window.hpp"

// The main menu.
class Menu : public Scene {
public:
    Menu() {
        image = IMG_Load("assets/sprites/mainmenu.png");
        assert(image != nullptr);
        texture = SDL_CreateTextureFromSurface(Window::renderer, image);
    }

    ~Menu() {
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(image);
    }

    void paint() override {
        SDL_RenderCopy(Window::renderer, texture, NULL, NULL);
    }

    void handle_event(Event e) override {}
    void tick() override {}
private:
    SDL_Surface* image;
    SDL_Texture* texture;
};