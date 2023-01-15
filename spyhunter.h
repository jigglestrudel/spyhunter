#pragma once

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "constants.h"
#include "drawing_functions.h"
#include "board.h"
#include "car.h"



typedef struct Game
{
	bool running, upArrow, downArrow, leftArrow, rightArrow, paused, gameOver, pointsStopped;
	int ticks, lastGameUpdate, lastGivenPoints, startTime, timeElapsed, pointsStoppedTime, lives, pointsCounted;
	long long unsigned score;
	SDL_Surface* screen;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	ColorEnum* colorEnum;
	TextureEnum* textureEnum;

	Board_t* board;

	Car_t* mainCar;
	Car_t* enemyCars;
	Car_t* npcCars;

} Game_t;

int game_init(Game_t* game);

void game_quit(Game_t* game);

void game_run(Game_t* game);
