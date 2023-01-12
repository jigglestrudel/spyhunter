#pragma once

#include "./SDL2-2.0.10/include/SDL.h"

typedef struct Board
{
	char* road_width;
	int delta_height;
	int tile_height;

}Board;

int get_width_at_y(Board board, int y);

void draw_board(Board board, SDL_Surface* screen);

void move_board(Board* board, int d_y);