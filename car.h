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
	CarTypeEnum lastTouchedBy;
	int deathTime;

} Car_t;

int car_init(Car_t* car, CarTypeEnum type, int x, int y);

void car_kill(Car_t* car);

void car_draw(Car_t* car, SDL_Surface* screen, TextureEnum* txtEnum);

void car_speed_up(Car_t* car);

void car_speed_up_reverse(Car_t* car);

void car_slow_down(Car_t* car);

void car_hit_break(Car_t* car);

void car_turn_left(Car_t* car);

void car_turn_right(Car_t* car);

void car_go_straight(Car_t* car);

void car_update(Car_t* car, int globalSpeed, Board_t* board);

void car_ai(Car_t* car, Car_t* other_car);

RoadStateEnum car_road_state(Car_t* car, Board_t* board);

void car_hitbox_update(Car_t* car, double globalSpeed, Board_t* board);

void car_check_collision(Car_t* car, Car_t* other_car);

void car_aim_at_target(Car_t* car, Car_t* target);

void car_respawn(Car_t* car, CarTypeEnum type, int x, int y, double vel = 0);

bool car_can_respawn(Car_t* car, int ticks);

void car_try_to_overtake(Car_t* car, Car_t* other_car);