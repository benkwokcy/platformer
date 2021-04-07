#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#include "Event.hpp"
#include "Menu.hpp"
#include "Window.hpp"
#include "Game.hpp"

using namespace std;

int main() {
    Window::start();
    Game game;
    while (game.is_running) {
        auto before = SDL_GetTicks();
        game.do_one_frame();
        if (auto after = SDL_GetTicks(); after - before < 16) {
            SDL_Delay(after - before);
        }
    }
    Window::stop();
    return 0;
}
