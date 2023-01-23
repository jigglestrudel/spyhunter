#include "car.h"

int car_init(Car_t* car, CarTypeEnum type, int x, int y)
{
	car->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!car->hitBox)
		return 1;
	car->hitBox->w = CAR_W;
	car->hitBox->h = CAR_H;

	car_respawn(car, type, x, y, 1);

    return 0;
}

void car_respawn(Car_t* car, CarTypeEnum type, int x, int y, double vel)
{
	car->carType = type;
	car->deathTime = 0;
	car->lastTouchedBy = WASTED;

	switch (type)
	{
	case WASTED:
	case NPC:
		car->hitBox->x = x;
		car->hitBox->y = y;
		car->speed = vel;
		car->turn = 0;
		break;
	case ENEMY:
		car->hitBox->x = x;
		car->hitBox->y = y;
		car->speed = vel;
		car->turn = 0;
		break;
	case PLAYER1:
		car->hitBox->x = x;
		car->hitBox->y = y;
		car->speed = 0;
		car->turn = 0;
		break;
	case PLAYER2:
		break;
	default:
		break;
	}
}

bool car_can_respawn(Car_t* car, int ticks)
{
	return (car->carType == WASTED && ticks - car->deathTime > RESPAWN_TIME) || abs(car->hitBox->y) > SCREEN_HEIGHT * 2;
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
	if (car->carType == ENEMY)
	{
		if (car->speed < MAX_ENEMY_SPEED)
			car->speed += ACCELERATION;
		return;
	}

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

void car_hitbox_update(Car_t* car, double globalSpeed, Board_t* board)
{
	car->hitBox->y -= car->speed - globalSpeed;
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

void car_ai(Car_t* car, Car_t* other_car)
{
	switch (car->carType)
	{
	case WASTED:
		car->speed = 0;
		car->turn = 0;
		
		break;
	case NPC:
		car_try_to_overtake(car, other_car);
		break;
	case ENEMY:
		if (other_car->carType == PLAYER1)
			car_aim_at_target(car, other_car);
		else
			car_try_to_overtake(car, other_car);
		break;
	default:
		break;
	}
}

void car_update(Car_t* car, int globalSpeed, Board_t* board)
{
	car_hitbox_update(car, globalSpeed, board);
	
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

bool car_check_collision_x(Car_t* car, Car_t* other_car, int marigin = 0)
{
	return ((car->hitBox->x + car->hitBox->w - marigin >= other_car->hitBox->x &&
			car->hitBox->x + car->hitBox->w - marigin <= other_car->hitBox->x + other_car->hitBox->w) ||
			(car->hitBox->x + marigin >= other_car->hitBox->x &&
			car->hitBox->x + marigin <= other_car->hitBox->x + other_car->hitBox->w));
}

bool car_check_collision_y_top(Car_t* car, Car_t* other_car, int marigin = 0)
{
	return (car->hitBox->y + marigin >= other_car->hitBox->y &&
			car->hitBox->y + marigin <= other_car->hitBox->y + other_car->hitBox->h);
}
bool car_check_collision_y_bottom(Car_t* car, Car_t* other_car, int marigin = 0)
{
	return (car->hitBox->y + car->hitBox->h - marigin >= other_car->hitBox->y &&
			car->hitBox->y + car->hitBox->h - marigin <= other_car->hitBox->y + other_car->hitBox->h);
}

bool car_check_collision_y(Car_t* car, Car_t* other_car, int marigin = 0)
{
	return car_check_collision_y_top(car, other_car, marigin) || car_check_collision_y_bottom(car, other_car, marigin);
}

bool car_check_side_collision(Car_t* car, Car_t* other_car)
{
	return car_check_collision_x(car, other_car, 0) && car_check_collision_y(car, other_car , HITBOX_MARIGIN);
}

bool car_check_top_collision(Car_t* car, Car_t* other_car)
{
	return car_check_collision_x(car, other_car, 0) && car_check_collision_y_top(car, other_car, -fabs(car->speed - other_car->speed));
}

bool car_check_bottom_collision(Car_t* car, Car_t* other_car)
{
	return car_check_collision_x(car, other_car, 0) && car_check_collision_y_bottom(car, other_car, -fabs(car->speed - other_car->speed));
}

void car_check_collision(Car_t* car, Car_t* other_car)
{
	if ((car_check_collision_x(car, other_car, HITBOX_MARIGIN) && car_check_collision_y(car, other_car, HITBOX_MARIGIN))||
		car_check_side_collision(car, other_car))
	{
		//printf("side kick\n");
		other_car->lastTouchedBy = car->carType;
		if (fabs(car->turn) - fabs(other_car->turn) > 0)
		{
			other_car->turn += OTHER_CAR_HITBACK *car->turn;
			car->turn = -car->turn;
		}
		else
		{
			car->turn += MAIN_CAR_HITBACK * other_car->turn;
			other_car->turn = -other_car->turn;
		}
		if (car_get_middle_x(car) > car_get_middle_x(other_car))
		{
			car->hitBox->x = other_car->hitBox->x + other_car->hitBox->w + 5;
		}
		else
		{
			car->hitBox->x = other_car->hitBox->x - car->hitBox->w - 5;
		}
	}
	else if (car_check_bottom_collision(car, other_car))
	{
		//printf("car_check_bottom_collision\n");
		other_car->carType = WASTED;
		other_car->lastTouchedBy = car->carType;

	}
	else if (car_check_top_collision(car, other_car))
	{
		//printf("car_check_top_collision\n");
		car->carType = WASTED;
		other_car->lastTouchedBy = car->carType;
	}

}

void car_go_back(Car_t* car)
{
	if (car->speed > 1)
	{
		car_hit_break(car);
	}
	else
	{
		car_speed_up_reverse(car);
	}
}

void car_try_to_overtake(Car_t* car, Car_t* other_car)
{
	if (car->speed > other_car->speed && car_get_middle_y(car) > car_get_middle_y(other_car) && car_check_collision_x(car, other_car, -20))
	{
		//printf("%p is_below_and_fast", car);
		if (car_check_collision_x(car, other_car, OVERTAKING_MARIGIN - fabs(car->speed - other_car->speed)))
		{
			//printf(" on colision course!!!\n");
			if (car_get_middle_x(car) > car_get_middle_x(other_car))
			{
				car_turn_right(car);
			}
			else
			{
				car_turn_left(car);
			}
		}
		//printf("\n");
	}
	else
	{
		car_go_straight(car);
	}
}

void car_aim_at_target(Car_t* car, Car_t* target)
{
	//printf("%d", car_check_collision_y(car, target, -10));
	if (car_check_collision_y(car, target))
	{
		
		if (car->hitBox->x < target->hitBox->x)
		{
			car_turn_right(car);
			car_turn_right(car);
		}
		else
		{
			car_turn_left(car);
			car_turn_left(car);
		}
	}
	else
	{
		car_go_straight(car);
		if (car_get_middle_y(car) - car_get_middle_y(target) < 0  && car->speed - target->speed > -5)
		{
			car_go_back(car);
		}

		else if (car_get_middle_y(car) > car_get_middle_y(target))
		{
			car_try_to_overtake(car, target);
			car_speed_up(car);
		}

	}
}
