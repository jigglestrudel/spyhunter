#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "drawing_functions.h"

typedef struct Board
{
	int* road_width;
	int road_width_length;
	int delta_height;
	int tile_height;

}Board_t;

int board_init(Board* board);

int board_get_width(Board* board, int y);

void board_draw(Board* board, SDL_Surface* screen, Uint32 grass_color, Uint32 road_color);

void board_move(Board* board, int d_y);

void board_kill(Board* board);