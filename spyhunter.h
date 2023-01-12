#pragma once

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "drawing_functions.h"
#include "board.h"


#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

typedef struct ColorEnum
{
	Uint32 black;
	Uint32 green;
	Uint32 red;
	Uint32 blue;
	Uint32 yellow;
	Uint32 magenta;
	Uint32 cyan;

} ColorEnum;

typedef struct Game
{
	bool running;
	int t1, frames;
	double delta, worldTime;
	SDL_Surface* screen, *charset;
	SDL_Surface* eti;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	ColorEnum* colorEnum;

} Game;



void color_init(Game* game);

void game_quit(Game* game);

int game_init(Game* game);

void game_time_update(Game* game);

void game_screen_update(Game* game);

void game_handle_events(Game* game);

void game_run(Game* game);