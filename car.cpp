#include "car.h"

int car_init(Car_t* car, CarTypeEnum type)
{
	car->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!car->hitBox) return 1;
	car->carType = type;
	car->hitBox->x = 300;
	car->hitBox->y = 300;
	car->hitBox->w = 30;
	car->hitBox->h = 48;
	car->speed = 0;
	car->turn = 0;
    return 0;
}

void car_kill(Car_t* car)
{
	SDL_free(car->hitBox);
	free(car);
}

void car_draw(Car_t* car, SDL_Surface* screen, TextureEnum* txtEnum)
{
	switch (car->carType)
	{
	case NPC:
		DrawSurface(screen, txtEnum->car_npc, car->hitBox->x, car->hitBox->y);
		break;
	case ENEMY:
		DrawSurface(screen, txtEnum->car_enemy, car->hitBox->x, car->hitBox->y);
		break;
	case PLAYER1:
		DrawSurface(screen, txtEnum->car_player, car->hitBox->x, car->hitBox->y);
		break;
	case PLAYER2:
		DrawSurface(screen, txtEnum->car_player2, car->hitBox->x, car->hitBox->y);
		break;
	case WASTED:
		DrawSurface(screen, txtEnum->exploded_car, car->hitBox->x, car->hitBox->y);
		break;

	default:
		break;
	}
}


void car_speed_up(Car_t* car)
{
	if (car->speed < MAX_SPEED)
		car->speed += ACCELERATION;
}

void car_speed_up_reverse(Car_t* car)
{
	if (car->speed > MAX_REVERSE_SPEED)
		car->speed += REVERSE_ACCELERATION;
}

void car_hit_break(Car_t* car)
{
	if (car->speed > 0)
		car->speed += BREAK_DECELERATION;
}

void car_slow_down(Car_t* car)
{
	if (car->speed > 0)
		car->speed += DECELERATION;
	if (car->speed < 0)
		car->speed += REVERSE_DECELERATION;
}


void car_turn_left(Car_t* car)
{
	if (car->turn > -MAX_TURN_SPEED)
		car->turn += -TURN_SPEED;
}
void car_turn_right(Car_t* car)
{
	if (car->turn < MAX_TURN_SPEED)
	car->turn += TURN_SPEED;
}
void car_go_straight(Car_t* car)
{
	if (car->turn > 0)
		car->turn += UNTURN_SPEED;
	if (car->turn < 0)
		car->turn += -UNTURN_SPEED;
}

void car_reset_speed(Car_t* car)
{
	car->speed = 0;
}

void car_hitbox_update(Car_t* car, double globalSpeed, Board_t* board)
{
	car->hitBox->y += car->speed - globalSpeed;
	car->hitBox->x += car->turn;
	if (car_road_state(car, board) == DEATH_ZONE)
	{
		car->carType = WASTED;
	}
}

int car_get_middle_x(Car_t* car)
{
	return car->hitBox->x + car->hitBox->w/2;
}

int car_get_middle_y(Car_t* car)
{
	return car->hitBox->y + car->hitBox->h/2;
}

RoadStateEnum car_road_state(Car_t* car, Board_t* board)
{
	int car_x = car_get_middle_x(car), car_y = car_get_middle_y(car);
	int road_left_edge = board_get_left_edge(board, car_y);
	int road_right_edge = board_get_right_edge(board, car_y);
	if (car_x > road_left_edge && car_x < road_right_edge)
		return RoadStateEnum(ROAD);
	else if (car_x > road_left_edge - OFF_ROAD_MARIGIN && car_x < road_right_edge + OFF_ROAD_MARIGIN)
		return RoadStateEnum(OFFROAD);
	else
		return RoadStateEnum(DEATH_ZONE);
}
