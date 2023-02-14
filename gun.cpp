#include "gun.h"

void bullet_init(Bullet* bullet)
{
	bullet->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	bullet->upgraded = false;
	bullet->shot = false;
	bullet->hitBox->x = -100;
	bullet->hitBox->y = -100;
}


void bullet_shoot(Bullet* bullet, int x, int y, int upgrade)
{
	bullet->shot = true;

	bullet->hitBox->x = x;
	bullet->hitBox->y = y;

	bullet->upgraded = upgrade;

	bullet->hitBox->w = BULLET_WIDTH;
	bullet->hitBox->h = BULLET_HEIGHT;
	if (upgrade)
	{
		bullet->hitBox->w = UPGRADE_BULLET_WIDTH;
		bullet->hitBox->h = UPGRADE_BULLET_HEIGHT;
	}
}

void bullet_update(Bullet* bullet, int offset)
{
	if(bullet->shot)
		bullet->hitBox->y -= BULLET_SPEED - offset;
}

void bullet_draw(Bullet* bullet, SDL_Surface* screen, int color)
{
	SDL_FillRect(screen, bullet->hitBox, color);
}

void bullet_kill(Bullet* bullet)
{
	SDL_free(bullet->hitBox);
	free(bullet);
}

void upgrade_init(Upgrade* entity)
{
	entity->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	upgrade_create(entity, SCREEN_WIDTH / 2, -1000, 1);
}

void upgrade_create(Upgrade* entity, int x, int y, int type)
{
	entity->hitBox->x = x;
	entity->hitBox->y = y;
	entity->hitBox->w = UPGRADE_ENTITY_SIZE;
	entity->hitBox->h = UPGRADE_ENTITY_SIZE;
	entity->picked_up = false;
	entity->type = type;
}

void upgrade_draw(Upgrade* entity, SDL_Surface* screen, SDL_Surface* txt)
{
	DrawSurface(screen, txt, entity->hitBox->x, entity->hitBox->y);
}

void upgrade_kill(Upgrade* entity)
{
	SDL_free(entity->hitBox);
	free(entity);
}

void gun_init(Gun* gun)
{
	gun->bullets = (Bullet*)malloc(MAX_BULLET_COUNT * sizeof(Bullet));
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		bullet_init(gun->bullets + i);
	}
	gun->upgrade_entity = (Upgrade*)malloc(sizeof(Upgrade));
	upgrade_init(gun->upgrade_entity);
	gun->ammo = 0;
	gun->last_shot = 0;
	gun->upgrade_type = 0;
	
}

void gun_shoot(Gun* gun, int start_x, int start_y, int tick)
{
	if (tick - gun->last_shot >= SHOOTING_COOLDOWN)
	{
		for (int i = 0; i < MAX_BULLET_COUNT; i++)
		{
			if ((gun->bullets + i)->shot == false)
			{
				bullet_shoot((gun->bullets + i), start_x, start_y, gun->upgrade_type);
				gun->last_shot = tick;
				if (gun->ammo > 0) gun->ammo--;
				if (gun->ammo == 0 && gun->upgrade_type != 0)
				{
					gun->upgrade_type = 0;
					upgrade_create(gun->upgrade_entity, SCREEN_WIDTH / 2, -1000, 1);
				}
				break;
			}
		}
	}
}

void gun_bullets_update(Gun* gun, int offset)
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		if ((gun->bullets + i)->shot)
		{
			bullet_update(gun->bullets + i, offset);
			if ((gun->bullets + i)->hitBox->y < 0)
				(gun->bullets + i)->shot = false;
		}
	}
}

bool bullet_check_collision(Bullet* bullet, SDL_Rect* hitBox)
{
	return ((bullet->hitBox->x + bullet->hitBox->w  >= hitBox->x &&
		bullet->hitBox->x + bullet->hitBox->w  <= hitBox->x + hitBox->w) ||
		(bullet->hitBox->x  >= hitBox->x &&
		bullet->hitBox->x <= hitBox->x + hitBox->w)) &&

		((bullet->hitBox->y - BULLET_SPEED >= hitBox->y &&
		bullet->hitBox->y - BULLET_SPEED <= hitBox->y +hitBox->h)||
		(bullet->hitBox->y + bullet->hitBox->h  >= hitBox->y &&
		bullet->hitBox->y + bullet->hitBox->h  <= hitBox->y + hitBox->h));
}

void upgrade_update(Upgrade* entity, int offset)
{
	entity->hitBox->y += offset;
}

bool upgrade_check_collision(Upgrade* entity, SDL_Rect* hitBox)
{
	return ((entity->hitBox->x + entity->hitBox->w >= hitBox->x &&
		entity->hitBox->x + entity->hitBox->w <= hitBox->x + hitBox->w) ||
		(entity->hitBox->x >= hitBox->x &&
		entity->hitBox->x <= hitBox->x + hitBox->w)) &&
		((entity->hitBox->y >= hitBox->y &&
		entity->hitBox->y <= hitBox->y + hitBox->h) ||
		(entity->hitBox->y + entity->hitBox->h >= hitBox->y &&
		entity->hitBox->y + entity->hitBox->h <= hitBox->y + hitBox->h));
}



bool gun_bullet_collision_check(Gun* gun, SDL_Rect* hitbox)
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		if ((gun->bullets + i)->shot)
		{
			if (bullet_check_collision(gun->bullets + i, hitbox))
			{
				(gun->bullets + i)->shot = false;
				return true;
			}
		}
	}
	return false;
}

void gun_upgrade_collision_check(Gun* gun, SDL_Rect* hitbox)
{
	if (upgrade_check_collision(gun->upgrade_entity, hitbox))
	{
		gun->upgrade_entity->picked_up = true;
		gun->upgrade_type = 1;
		gun->ammo = UPGRADE_AMMO;
	}
}

void gun_draw(Gun* gun, SDL_Surface* screen, int bulletcolor, SDL_Surface* upgrade_txt)
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		if ((gun->bullets + i)->shot)
		{
			bullet_draw((gun->bullets + i), screen, bulletcolor);
		}
	}

	if (!(gun->upgrade_entity->picked_up))
		upgrade_draw(gun->upgrade_entity, screen, upgrade_txt);
}
