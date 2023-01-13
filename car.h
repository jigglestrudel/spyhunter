#pragma once

#include "drawing_functions.h"

enum CarTypeEnum
{
	NPC,
	ENEMY,
	PLAYER1,
	PLAYER2,
};


typedef struct Car
{
	SDL_Rect* hitBox;
	int dx, dy;
	CarTypeEnum carType;

} Car_t;

int car_init(Car_t* car, CarTypeEnum type);

void car_draw(Car_t* car, SDL_Surface* screen, TextureEnum* txtEnum);

void car_change_delta(Car_t* car, int dx, int dy);

void car_reset_delta(Car_t* car);

void car_update(Car_t* car);