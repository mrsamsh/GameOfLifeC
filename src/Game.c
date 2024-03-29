//
//
// Game.c
//
// GameOfLife
//
// Created by Usama Alshughry 08.06.2021
//
// All rights reserved 2021
//

#define CALCULATE_CELL(cell) (*(cell.nw) + *(cell.n) + *(cell.ne) + *(cell.w) + *(cell.e) + *(cell.sw) + *(cell.s) + *(cell.se))

#include "Game.h"
#include <time.h>
#include <math.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct Cell {
	uint32_t v;
	uint32_t f;
	uint32_t *nw;
	uint32_t *n;
	uint32_t *ne;
	uint32_t *w;
	uint32_t *e;
	uint32_t *sw;
	uint32_t *s;
	uint32_t *se;
} Cell;

int width, height, side, starting_cells;
Cell *state1;
Cell *state2;
Cell *current;
Cell *next;
SDL_Rect fullscreen, *rects;

static inline int Cell_Calculate(const Cell *cell) {
	return *(cell->nw) + *(cell->n) + *(cell->ne) + *(cell->w) + *(cell->e) + *(cell->sw) + *(cell->s) + *(cell->se);
}

void Game_Init(int grid_width, int grid_height, int cell_side, int starting) {
	width = grid_width;
	height = grid_height;
	side = cell_side;
	starting_cells = starting;
	state1 = malloc(sizeof(Cell) * width * height);
	state2 = malloc(sizeof(Cell) * width * height);
	current = state1;
	next = state2;
	fullscreen.x = 0;
	fullscreen.y = 0;
	fullscreen.w = width * side;
	fullscreen.h = height * side;

	for (int y = 1; y < height - 1; ++y) {
		for (int x = 1; x < width - 1; ++x) {
			int i = x + y * width;
			state1[i].w  = &state1[i - 1].v;
			state1[i].e  = &state1[i + 1].v;
			state1[i].n  = &state1[i - width].v;
			state1[i].s  = &state1[i + width].v;
			state1[i].nw = &state1[i - width - 1].v;
			state1[i].ne = &state1[i - width + 1].v;
			state1[i].sw = &state1[i + width - 1].v;
			state1[i].se = &state1[i + width + 1].v;
			
			state2[i].w  = &state2[i - 1].v;
			state2[i].e  = &state2[i + 1].v;
			state2[i].n  = &state2[i - width].v;
			state2[i].s  = &state2[i + width].v;
			state2[i].nw = &state2[i - width - 1].v;
			state2[i].ne = &state2[i - width + 1].v;
			state2[i].sw = &state2[i + width - 1].v;
			state2[i].se = &state2[i + width + 1].v;
		}
	}
	for (int x = 1; x < width - 1; ++x) {
		int itop = x;
		int ibot = x + width * (height - 1);
		state1[itop].w  = &state1[itop - 1].v;
		state1[itop].e  = &state1[itop + 1].v;
		state1[itop].s  = &state1[itop + width].v;
		state1[itop].n  = &state1[ibot].v;
		state1[itop].sw = &state1[itop + width - 1].v;
		state1[itop].se = &state1[itop + width + 1].v;
		state1[itop].nw = &state1[ibot - 1].v;
		state1[itop].ne = &state1[ibot + 1].v;
		
		state2[itop].w  = &state2[itop - 1].v;
		state2[itop].e  = &state2[itop + 1].v;
		state2[itop].s  = &state2[itop + width].v;
		state2[itop].n  = &state2[ibot].v;
		state2[itop].sw = &state2[itop + width - 1].v;
		state2[itop].se = &state2[itop + width + 1].v;
		state2[itop].nw = &state2[ibot - 1].v;
		state2[itop].ne = &state2[ibot + 1].v;
		
		state1[ibot].w  = &state1[ibot - 1].v;
		state1[ibot].e  = &state1[ibot + 1].v;
		state1[ibot].n  = &state1[ibot - width].v;
		state1[ibot].s  = &state1[itop].v;
		state1[ibot].nw = &state1[ibot - width - 1].v;
		state1[ibot].ne = &state1[ibot - width + 1].v;
		state1[ibot].sw = &state1[itop - 1].v;
		state1[ibot].se = &state1[itop + 1].v;
		
		state2[ibot].w  = &state2[ibot - 1].v;
		state2[ibot].e  = &state2[ibot + 1].v;
		state2[ibot].n  = &state2[ibot - width].v;
		state2[ibot].s  = &state2[itop].v;
		state2[ibot].nw = &state2[ibot - width - 1].v;
		state2[ibot].ne = &state2[ibot - width + 1].v;
		state2[ibot].sw = &state2[itop - 1].v;
		state2[ibot].se = &state2[itop + 1].v;
	}
	
	for (int y = 1; y < height - 1; ++y) {
		int il = y * width;
		int ir = y * width + width - 1;
		
		state1[il].n  = &state1[il - width].v;
		state1[il].s  = &state1[il + width].v;
		state1[il].e  = &state1[il + 1].v;
		state1[il].w  = &state1[ir].v;
		state1[il].ne = &state1[il - width + 1].v;
		state1[il].se = &state1[il + width + 1].v;
		state1[il].nw = &state1[ir - width].v;
		state1[il].sw = &state1[ir + width].v;
		
		state2[il].n  = &state2[il - width].v;
		state2[il].s  = &state2[il + width].v;
		state2[il].e  = &state2[il + 1].v;
		state2[il].w  = &state2[ir].v;
		state2[il].ne = &state2[il - width + 1].v;
		state2[il].se = &state2[il + width + 1].v;
		state2[il].nw = &state2[ir - width].v;
		state2[il].sw = &state2[ir + width].v;
		
		state1[ir].n  = &state1[ir - width].v;
		state1[ir].s  = &state1[ir + width].v;
		state1[ir].w  = &state1[ir - 1].v;
		state1[ir].e  = &state1[il].v;
		state1[ir].nw = &state1[ir - width - 1].v;
		state1[ir].sw = &state1[ir + width - 1].v;
		state1[ir].ne = &state1[il - width].v;
		state1[ir].se = &state1[il + width].v;
		
		state2[ir].n  = &state2[ir - width].v;
		state2[ir].s  = &state2[ir + width].v;
		state2[ir].w  = &state2[ir - 1].v;
		state2[ir].e  = &state2[il].v;
		state2[ir].nw = &state2[ir - width - 1].v;
		state2[ir].sw = &state2[ir + width - 1].v;
		state2[ir].ne = &state2[il - width].v;
		state2[ir].se = &state2[il + width].v;
	}
	
	// state1[0]
	state1[0].e  = &state1[1].v;
	state1[0].w  = &state1[width - 1].v;
	state1[0].s  = &state1[width].v;
	state1[0].n  = &state1[width * (height - 1)].v;
	state1[0].se = &state1[width + 1].v;
	state1[0].sw = &state1[width + width - 1].v;
	state1[0].ne = &state1[width * (height - 1) + 1].v;
	state1[0].nw = &state1[width * height - 1].v;
	
	state2[0].e  = &state2[1].v;
	state2[0].w  = &state2[width - 1].v;
	state2[0].s  = &state2[width].v;
	state2[0].n  = &state2[width * (height - 1)].v;
	state2[0].se = &state2[width + 1].v;
	state2[0].sw = &state2[width + width - 1].v;
	state2[0].ne = &state2[width * (height - 1) + 1].v;
	state2[0].nw = &state2[width * height - 1].v;
	
	// state1[width - 1]
	state1[width - 1].w  = &state1[width - 2].v;
	state1[width - 1].e  = &state1[0].v;
	state1[width - 1].n  = &state1[width * height - 1].v;
	state1[width - 1].s  = &state1[width * 2 - 1].v;
	state1[width - 1].sw = &state1[width * 2 - 2].v;
	state1[width - 1].se = &state1[width].v;
	state1[width - 1].nw = &state1[width * height - 2].v;
	state1[width - 1].ne = &state1[width * (height - 1)].v;
	
	state2[width - 1].w  = &state2[width - 2].v;
	state2[width - 1].e  = &state2[0].v;
	state2[width - 1].n  = &state2[width * height - 1].v;
	state2[width - 1].s  = &state2[width * 2 - 1].v;
	state2[width - 1].sw = &state2[width * 2 - 2].v;
	state2[width - 1].se = &state2[width].v;
	state2[width - 1].nw = &state2[width * height - 2].v;
	state2[width - 1].ne = &state2[width * (height - 1)].v;
	
	// state1[width * (height - 1)]
	state1[width * (height - 1)].n  = &state1[width * (height - 2)].v;
	state1[width * (height - 1)].s  = &state1[0].v;
	state1[width * (height - 1)].e  = &state1[width * (height - 1) + 1].v;
	state1[width * (height - 1)].w  = &state1[width * height - 1].v;
	state1[width * (height - 1)].ne = &state1[width * (height - 2) + 1].v;
	state1[width * (height - 1)].nw = &state1[width * (height - 1) - 1].v;
	state1[width * (height - 1)].se = &state1[1].v;
	state1[width * (height - 1)].sw = &state1[width - 1].v;
	
	state2[width * (height - 1)].n  = &state2[width * (height - 2)].v;
	state2[width * (height - 1)].s  = &state2[0].v;
	state2[width * (height - 1)].e  = &state2[width * (height - 1) + 1].v;
	state2[width * (height - 1)].w  = &state2[width * height - 1].v;
	state2[width * (height - 1)].ne = &state2[width * (height - 2) + 1].v;
	state2[width * (height - 1)].nw = &state2[width * (height - 1) - 1].v;
	state2[width * (height - 1)].se = &state2[1].v;
	state2[width * (height - 1)].sw = &state2[width - 1].v;
	
	// state1[width * height - 1]
	state1[width * height - 1].n  = &state1[width * (height - 1) - 1].v;
	state1[width * height - 1].s  = &state1[width - 1].v;
	state1[width * height - 1].e  = &state1[width * (height - 1)].v;
	state1[width * height - 1].w  = &state1[width * height - 2].v;
	state1[width * height - 1].nw = &state1[width * (height - 1) - 2].v;
	state1[width * height - 1].ne = &state1[width * (height - 2)].v;
	state1[width * height - 1].sw = &state1[width - 2].v;
	state1[width * height - 1].se = &state1[0].v;
	
	state2[width * height - 1].n  = &state2[width * (height - 1) - 1].v;
	state2[width * height - 1].s  = &state2[width - 1].v;
	state2[width * height - 1].e  = &state2[width * (height - 1)].v;
	state2[width * height - 1].w  = &state2[width * height - 2].v;
	state2[width * height - 1].nw = &state2[width * (height - 1) - 2].v;
	state2[width * height - 1].ne = &state2[width * (height - 2)].v;
	state2[width * height - 1].sw = &state2[width - 2].v;
	state2[width * height - 1].se = &state2[0].v;

	rects = malloc(sizeof(SDL_Rect) * width * height);

	for (int i = 0; i < width * height; ++i) {
		int x = (i % width) * side;
		int y = (i / width) * side;
		rects[i] = (SDL_Rect){ x, y, side - 1, side -1 };
	}
}

void Game_GenerateRandom() {
	srand(time(NULL));
	for (int i = 0; i < width * height; ++i) {
		current[i].v = 0;
		next[i].v = 0;
		current[i].f = 0;
		next[i].f = 0;
	}
	for (int i = 0; i < starting_cells; ++i) {
		int x = rand() % width;
		int y = rand() % height;
		current[x + y * width].v = 1;
	}
	for (int i = 0; i < width * height; ++i) {
		int count = CALCULATE_CELL(current[i]);
		if ((count < 2 || count > 3) && current[i].v == 1) {
			next[i].v = 0;
		} else if (count == 3 && current[i].v != 1) {
			next[i].v = 1;
		} else {
			next[i].v = current[i].v;
		}
	}
	Game_Swap();
}

void Game_InsertPattern(int xpos, int ypos, int width, int height, const int8_t *pattern) {
	int pi = 0;
	for (int y = ypos; y < ypos + height; ++y) {
		for (int x = xpos; x < xpos + width; ++x) {
			current[x + y * width].v = pattern[pi++];
		}
	}
}

void Game_Swap() {
	Cell *temp = current;
	current = next;
	next = temp;
}

void Game_EvaluateCells(SDL_Renderer *renderer, int paused) {
	if (!paused) {
		for (int i = 0; i < width * height; ++i) {
			int count = CALCULATE_CELL(current[i]);

			// switch (count) {
			// 	case 3:
			// 		next[i].v = 1;
			// 		next[i].f = 0;
			// 		continue;
			// 	case 2:
			// 		next[i].v = current[i].v;
			// 		continue;
			// 	default:
			// 		next[i].v = 0;
			// 		switch (current[i].v) {
			// 			case 1:
			// 				next[i].f = 60;
			// 				break;
			// 			default:
			// 				next[i].f = MAX(0, current[i].f - 1);
			// 		}
			// 		break;
			// }

			// switch (current[i].v) {
			// 	case 1:
			// 	{
			// 		switch (count) {
			// 			case 2:
			// 			case 3:
			// 				next[i].v = 1;
			// 				break;
			// 			default:
			// 				next[i].v = 0;
			// 				next[i].f = 60;
			// 				break;
			// 		}
			// 	} break;
			// 	default:
			// 	{
			// 		switch (count) {
			// 			case 3:
			// 				next[i].v = 1;
			// 				next[i].f = 0;
			// 				break;
			// 			default:
			// 				next[i].v = 0;
			// 				switch (current[i].f) {
			// 					case 0:
			// 						next[i].f = 0;
			// 						break;
			// 					default:
			// 						next[i].f = current[i].f -1;
			// 				}
			// 				break;
			// 		}
			// 	} break;
			// }

			if ((count < 2 || count > 3) && current[i].v == 1) {
				next[i].v = 0;
				next[i].f = 20;
			} else if (count == 3 && current[i].v != 1) {
				next[i].v = 1;
				next[i].f = 0;
			} else {
				next[i].v = current[i].v;
				next[i].f = MAX(current[i].f - 1, 0);
			}
		}
	}
	Game_Draw(renderer);

	if (!paused)
		Game_Swap();
}


void Game_Draw(void *r) {
	SDL_Renderer *renderer = (SDL_Renderer *)r;
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
	for (int i = 0; i < width * height; ++i) {

		if (current[i].v == 1) {
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			SDL_RenderFillRect(renderer, &rects[i]);
			continue;
		}
		switch (current[i].f) {
			case 20:
			case 19:
			case 18:
			case 17:
				SDL_SetRenderDrawColor(renderer, 0x25, 0x25, 0xa0, 0xff);
				SDL_RenderFillRect(renderer, &rects[i]);
				continue;
			case 16:
			case 15:
			case 14:
			case 13:
				SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x80, 0xff);
				SDL_RenderFillRect(renderer, &rects[i]);
				continue;
			case 12:
			case 11:
			case 10:
			case 9:
				SDL_SetRenderDrawColor(renderer, 0x15, 0x15, 0x60, 0xff);
				SDL_RenderFillRect(renderer, &rects[i]);
				continue;
			case 8:
			case 7:
			case 6:
			case 5:
				SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x40, 0xff);
				SDL_RenderFillRect(renderer, &rects[i]);
				continue;
			case 4:
			case 3:
			case 2:
			case 1:
				SDL_SetRenderDrawColor(renderer, 0x05, 0x05, 0x20, 0xff);
				SDL_RenderFillRect(renderer, &rects[i]);
				continue;
			default:
				continue;
				
		}
	}
}

void Game_Destroy() {
	free(rects);
	free(state1);
	free(state2);
}
