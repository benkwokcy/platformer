#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>
#include <utility>

class Window {
public:
    static inline SDL_Window* window;
    static inline SDL_Renderer* renderer;
    constexpr static int width = 640;
    constexpr static int height = 480;
    constexpr static char name[] = "Platformer";

    // Initialize SDL libraries
    static void start() {
        if (auto res = SDL_Init(SDL_INIT_VIDEO); res == -1) {
            throw std::runtime_error("Failed SDL_Init.");
        }
        if (window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); window == nullptr) {
            throw std::runtime_error("Failed SDL_CreateWindow.");
        }
        if (renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC); renderer == nullptr) {
            throw std::runtime_error("Failed SDL_CreateRenderer.");
        }
        if (!IMG_Init(IMG_INIT_PNG)) {
            throw std::runtime_error("Failed IMG_Init.");
        }
    }

    // Shut down SDL libraries
    static void stop() {
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    constexpr static int center_x() { 
        return width / 2; 
    }

    constexpr static int center_y() { 
        return height / 2; 
    }
};
