#include "Game.h"
#include <stdio.h>

typedef enum {
	false = 0,
	true = 1
} bool;

int8_t hammerHead[] = {
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1,
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1,
	0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int8_t cloverField[] = {
	0, 0, 0, 1, 0, 1, 0, 0, 0,
	0, 1, 1, 1, 0, 1, 1, 1, 0,
	1, 0, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 1, 0, 1,
	0, 1, 1, 0, 1, 0, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 1, 0, 1, 1, 0,
	1, 0, 1, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 1, 1, 1, 0, 1, 1, 1, 0,
	0, 0, 0, 1, 0, 1, 0, 0, 0
};
int8_t glider[] = {
	0, 1, 0,
	1, 0, 0,
	1, 1, 1
};

int8_t square[] = {
	1, 1,
	1, 1
};

struct { int window_width, window_height, side, starting_cells_percent, fps, display, hidpi; } c;

int main()
{
	FILE *file;
	file = fopen("config.txt", "r");
	char dummy[20];
	fscanf(file, "%[^=]=%d %[^=]=%d %[^=]=%d %[^=]=%d %[^=]=%d %[^=]=%d %[^=]=%d",
			dummy, &c.window_width,
			dummy, &c.window_height,
			dummy, &c.side,
			dummy, &c.starting_cells_percent,
			dummy, &c.fps,
			dummy, &c.display,
			dummy, &c.hidpi);
	int window_width = c.window_width;
	int window_height = c.window_height;
	int side = c.side;
	int grid_width = window_width / side;
	int grid_height = window_height / side;
	int starting_cells = c.starting_cells_percent * grid_width * grid_height / 100;
	int hidpi = c.hidpi;

	Game_Init(grid_width, grid_height, side, starting_cells);

	Game_GenerateRandom();

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(
			"Game of Life",
			SDL_WINDOWPOS_CENTERED_DISPLAY(c.display),
			SDL_WINDOWPOS_CENTERED_DISPLAY(c.display),
			hidpi == 1 ? (window_width / 2) : window_width,
			hidpi == 1 ? (window_height / 2) : window_height,
			SDL_WINDOW_SHOWN
			| SDL_WINDOW_ALLOW_HIGHDPI
			| SDL_WINDOW_FULLSCREEN_DESKTOP
			| SDL_WINDOW_RESIZABLE
			);

	SDL_Renderer* renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
			| SDL_RENDERER_PRESENTVSYNC
			);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	uint32_t last, current;
	uint32_t mspf = 1000 / c.fps;
	last = SDL_GetTicks();
	bool running = true;
	unsigned int counter = 0;
	uint32_t time = SDL_GetTicks();
	bool paused = false;
	bool shadows = true;
	while (running) {
		counter++;
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			} else if (event.type== SDL_WINDOWEVENT) {

				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					int x, y;
					SDL_GetWindowSize(window, &x, &y);
					SDL_Rect viewport;
					float ratio = (float)window_width / (float)window_height;
					float newRation = (float)x / (float)y;
					float scale;
					if (newRation > ratio) {
						scale = (float)y / (float)window_height;
						viewport.w = window_width;
						viewport.h = window_height;
						viewport.x = (int)(x / scale - viewport.w) / (hidpi == 1 ? 2 : 1);
						viewport.y = 0;
					} else {
						scale = (float)x / (float)window_width;
						viewport.w = window_width;
						viewport.h = window_height;
						viewport.x = 0;
						viewport.y = (int)(y / scale - viewport.h) / (hidpi == 1 ? 2 : 1);
					}
					SDL_RenderSetScale(renderer, scale * (hidpi == 1 ? 2.f : 1.f), scale * (hidpi == 1 ? 2.f : 1.f));
					SDL_RenderSetViewport(renderer, &viewport);
				}
			} else if (event.type == SDL_KEYUP) {

				if (event.key.keysym.sym == SDLK_RETURN) {
					Game_GenerateRandom();
				} else if (event.key.keysym.sym == SDLK_SPACE) {
					paused = !paused;
				} else if (event.key.keysym.sym == SDLK_s) {
					shadows = !shadows;
				}
			}
		}

		uint32_t beforeEval = SDL_GetTicks();
		Game_EvaluateCells(renderer, paused);
		uint32_t afterEval = SDL_GetTicks();

		SDL_RenderPresent(renderer);
		current = SDL_GetTicks();
		uint32_t delta = current - last;
		if (mspf > delta) {
			SDL_Delay(mspf - delta);
		}
		last = SDL_GetTicks();
	}
	time = SDL_GetTicks() - time;
	printf("Average = %f\n", (double)time / (double)counter);
	Game_Destroy();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return EXIT_SUCCESS;
}
