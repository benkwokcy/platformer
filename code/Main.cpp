#include <SDL2/SDL.h>

#include <iostream>

#include "Window.hpp"
#include "Game.hpp"

using namespace std;

class PerformanceMonitor {
public:
    int total_time_in_frame = 0;
    int max_time_in_frame = 0;
    int num_frames = 0;
    const char* A = "Average time per frame: ";
    const char* B = " ms | Maximum time per frame: ";
    const char* C = " ms\n";

    void update(int time_in_frame) {
        max_time_in_frame = std::max(max_time_in_frame, time_in_frame);
        total_time_in_frame += time_in_frame;
        num_frames++;

        if (num_frames == 60) {
            std::cout << A << total_time_in_frame / num_frames << B << max_time_in_frame << C;
            num_frames = 0;
            total_time_in_frame = 0;
            max_time_in_frame = 0;
        }
    }
};

int main() {
    Window::start();
    PerformanceMonitor monitor;
    Game game;

    while (game.is_running()) {
        auto before = SDL_GetTicks();
        game.tick();
        auto after = SDL_GetTicks();
        if (auto ms = after - before; ms < 16) {
            SDL_Delay(16 - ms);
        }
        monitor.update(after - before);
    }

    Window::stop();
    return 0;
}