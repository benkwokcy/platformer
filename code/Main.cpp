#include <SDL2/SDL.h>

#include <iostream>

#include "Window.hpp"
#include "Game.hpp"

using namespace std;

int main() {
    Window::start();

    int total_time_in_frame = 0;
    int num_frames = 0;

    Game game;
    while (game.is_running()) {
        auto before = SDL_GetTicks();
        game.tick();
        auto after = SDL_GetTicks();
        if (auto ms = after - before; ms < 16) {
            SDL_Delay(16 - ms);
        }
        {
            total_time_in_frame += after - before;
            num_frames++;
        }
    }
    
    std::cout << "Average time per frame: " << total_time_in_frame / num_frames << " ms\n";

    Window::stop();
    return 0;
}