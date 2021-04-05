#include <SDL2/SDL.h>
#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
	// Create Window and Renderer
	SDL_Window* window;
	SDL_Renderer* renderer;
	char window_name[] = "Platformer";
	int window_width = 800;
	int window_height = 600;

	if (auto res = SDL_Init(SDL_INIT_VIDEO); res == -1) {
		throw runtime_error("Failed SDL_Init.");
	}
	if (window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN); window == nullptr) {
		throw runtime_error("Failed SDL_CreateWindow.");
	}
	if (renderer = SDL_CreateRenderer(window, -1, 0); renderer == nullptr) {
		throw runtime_error("Failed SDL_CreateRenderer.");
	}

	// Main Loop
	bool is_running = true;
	SDL_Event event;

	while (is_running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				is_running = false;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	// Destroy Window and Renderer
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
