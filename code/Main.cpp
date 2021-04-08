#include <SDL2/SDL.h>

#include <iostream>

#include "Window.hpp"
#include "Game.hpp"

using namespace std;

int main() {
    Window::start();

    Game game;
    while (game.is_running()) {
        auto before = SDL_GetTicks();
        game.do_one_frame();
        auto after = SDL_GetTicks();
        if (auto ms = after - before; ms < 16) {
            SDL_Delay(ms);
        }
    }
    
    Window::stop();
    return 0;
}
