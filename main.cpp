#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main()
{
	if (auto res = SDL_Init(SDL_INIT_VIDEO); res == -1) { assert(false); };
	auto window = SDL_CreateWindow("GAME_NAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	assert(window != nullptr);
	auto renderer = SDL_CreateRenderer(window, -1, 0);

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

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
