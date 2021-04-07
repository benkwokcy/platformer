#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>

using namespace std;

class Window {
   public:
    static inline SDL_Window* window;
    static inline SDL_Renderer* renderer;
    constexpr static int window_width = 640;
    constexpr static int window_height = 480;
    constexpr static char window_name[] = "Platformer";

    // Initialize SDL libraries
    static void start() {
        if (auto res = SDL_Init(SDL_INIT_VIDEO); res == -1) {
            throw runtime_error("Failed SDL_Init.");
        }
        if (window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN); window == nullptr) {
            throw runtime_error("Failed SDL_CreateWindow.");
        }
        if (renderer = SDL_CreateRenderer(window, -1, 0); renderer == nullptr) {
            throw runtime_error("Failed SDL_CreateRenderer.");
        }
        if (!IMG_Init(IMG_INIT_PNG)) {
            throw runtime_error("Failed IMG_Init.");
        }
    }

    // Shut down SDL libraries
    static void stop() {
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};
