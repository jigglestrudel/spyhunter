#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "constants.h"
#include "drawing_functions.h"


typedef struct Board
{
	int* road_width;
	int road_width_length;
	int delta_height;
	int tile_height;
	int skipped;
	int grass_marigin;

}Board_t;

int board_init(Board* board);

int road_load_file(Board* board, const char* path);

int board_get_width(Board* board, int y);

int board_get_left_edge(Board_t* board, int y);

int board_get_right_edge(Board_t* board, int y);

void board_draw(Board* board, SDL_Surface* screen, SDL_Surface* grass, SDL_Surface* tree, Uint32 road_color, Uint32 stripe_color);

void board_move(Board* board, int d_y);

void board_kill(Board* board);