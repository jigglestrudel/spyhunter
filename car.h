#pragma once

#include "constants.h"
#include "drawing_functions.h"
#include "board.h"


enum CarTypeEnum
{
	NPC,
	ENEMY,
	PLAYER1,
	PLAYER2,
	WASTED
};

enum RoadStateEnum
{
	ROAD,
	OFFROAD,
	DEATH_ZONE
};


typedef struct Car
{
	SDL_Rect* hitBox;
	double speed, turn;
	CarTypeEnum carType;

} Car_t;

int car_init(Car_t* car, CarTypeEnum type);

void car_kill(Car_t* car);

void car_draw(Car_t* car, SDL_Surface* screen, TextureEnum* txtEnum);

void car_speed_up(Car_t* car);

void car_speed_up_reverse(Car_t* car);

void car_slow_down(Car_t* car);

void car_hit_break(Car_t* car);

void car_turn_left(Car_t* car);

void car_turn_right(Car_t* car);

void car_go_straight(Car_t* car);

void car_reset_speed(Car_t* car);

void car_hitbox_update(Car_t* car, double globalSpeed, Board_t* board);

RoadStateEnum car_road_state(Car_t* car, Board_t* board);