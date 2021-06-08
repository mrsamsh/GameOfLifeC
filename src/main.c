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

struct { int window_width, window_height, side, fps; } c;

int main()
{
	FILE *file;
	file = fopen("config.txt", "r");
	fscanf(file, "%d\n%d\n%d\n%d\n", &c.window_width, &c.window_height, &c.side, &c.fps);
		
	int window_width = c.window_width * 2;
	int window_height = c.window_height * 2;
	int side = c.side;
	int grid_width = window_width / side;
	int grid_height = window_height / side;
	int starting_cells = (window_width * window_height) / (side * 70);

	Game_Init(grid_width, grid_height, side, starting_cells);
	
	Game_GenerateRandom();
//    game.insertPattern(50, 50,  9, 11, &cloverField[0]);
//    game.insertPattern(200, 100, 18, 16, &hammerHead[0]);
	
//	game.insertPattern(100, 100, 3, 3, &glider[0]);
//	game.insertPattern(88, 110, 2, 2, &square[0]);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(
										  "Game of Life",
										  SDL_WINDOWPOS_CENTERED_DISPLAY(2),
										  SDL_WINDOWPOS_CENTERED_DISPLAY(2),
										  window_width / 2,
										  window_height/ 2,
										  SDL_WINDOW_SHOWN
										  | SDL_WINDOW_ALLOW_HIGHDPI
										  | SDL_WINDOW_FULLSCREEN//_DESKTOP
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
//	if (SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengl", SDL_HINT_OVERRIDE) == SDL_TRUE) printf("Done\n");
	Uint32 last, current;
	Uint32 mspf = 1000 / c.fps;
	last = SDL_GetTicks();
	bool running = true;
	unsigned int counter = 0;
	uint32_t time = SDL_GetTicks();
	bool paused = false;
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
						viewport.x = (int)(x / scale - viewport.w) / 2;
						viewport.y = 0;
					} else {
						scale = (float)x / (float)window_width;
						viewport.w = window_width;
						viewport.h = window_height;
						viewport.x = 0;
						viewport.y = (int)(y / scale - viewport.h) / 2;
					}
					SDL_RenderSetScale(renderer, scale * 2.f, scale * 2.f);
					SDL_RenderSetViewport(renderer, &viewport);
				}
			} else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_RETURN) {
					Game_GenerateRandom();
				} else if (event.key.keysym.sym == SDLK_SPACE) {
					paused = !paused;
				}
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x66);
		SDL_RenderClear(renderer);
		if (!paused) {
			Game_EvaluateCells();
			Game_Swap();
		}
		Game_Draw(renderer);
		
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