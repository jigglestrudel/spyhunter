#pragma once

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "drawing_functions.h"
#include "board.h"
#include "car.h"


#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

enum SpeedStages
{
	ZERO = 0,
	SLOW = 10,
	MEDIUM = 20,
	FAST = 30,
	REVERSE = -10
};

typedef struct Game
{
	bool running, upArrow, downArrow;
	int ticks, lastGameUpdate, frames, globalSpeed, speedT1;
	double delta, worldTime;
	SDL_Surface* screen;
	SDL_Surface* charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	ColorEnum* colorEnum;
	TextureEnum* textureEnum;

	Board_t* board;

	Car_t* main_car;
	Car_t* enemy_cars;
	Car_t* npc_cars;

} Game_t;



void color_init(Game_t* game);

int texture_load(Game_t* game);

void game_quit(Game_t* game);

int game_init(Game_t* game);

void game_time_update(Game_t* game);

void game_screen_update(Game_t* game);

void game_handle_events(Game_t* game);

void game_speed_update(Game_t* game);

void game_logic_update(Game_t* game);
