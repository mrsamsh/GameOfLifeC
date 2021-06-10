//
//  Game.h
//  GameOfLife
//
//  Created by Usama Alshughry on 31.08.2019.
//  Copyright © 2019 Usama Alshughry. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <SDL2/SDL.h>
#include <stdint.h>

void Game_Init(int grid_width, int grid_height, int side, int starting_cells);

void Game_GenerateRandom();
void Game_InsertPattern(int xpos, int ypos, int width, int height, const int8_t *pattern);
void *Game_Draw(void *);
void Game_EvaluateCells(SDL_Renderer *, int paused);
void Game_Swap();
void Game_Destroy();

#endif /* Game_h */
