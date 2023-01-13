#include "car.h"

int car_init(Car_t* car, CarTypeEnum type)
{
	car->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	car->carType = type;
	car->hitBox->x = 300;
	car->hitBox->y = 300;
	car->hitBox->w = 30;
	car->hitBox->h = 48;
	car->dx = 0;
	car->dy = 0;
    return 0;
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

	default:
		break;
	}
}

void car_change_delta(Car_t* car, int dx, int dy)
{
	car->dx = dx;
	car->dy = dy;
}

void car_reset_delta(Car_t* car)
{
	car->dx = 0;
	car->dy = 0;
}

void car_update(Car_t* car)
{
	car->hitBox->x += car->dx;
	car->hitBox->y += car->dy;
}

