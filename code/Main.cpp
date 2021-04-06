#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <filesystem>

#include "Window.hpp"

using namespace std;

int main() {
    Window::start();

    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* image = IMG_Load("assets/sprites/mainmenu.png");
    assert(image != nullptr);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, image);

    bool is_running = true;
    SDL_Event event;

    while (is_running) {
        auto before = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                is_running = false;
            }
        }
        SDL_RenderCopy(Window::renderer, texture, NULL, NULL);
        SDL_RenderPresent(Window::renderer);
        if (auto after = SDL_GetTicks(); after - before < 16) {
            SDL_Delay(after - before);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    IMG_Quit();

    Window::stop();

    return 0;
}
