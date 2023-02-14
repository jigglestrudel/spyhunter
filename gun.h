#pragma once

#include "constants.h"
#include "drawing_functions.h"

typedef struct Bullet
{
	SDL_Rect* hitBox;
	int upgraded;
	bool shot;
}Bullet;

typedef struct Upgrade
{
	SDL_Rect* hitBox;
	bool picked_up;
	int type;
}Upgrade;

typedef struct Gun
{
	Bullet* bullets;
	Upgrade* upgrade_entity;
	int ammo;
	int last_shot;
	int upgrade_type;
}Gun;



void bullet_shoot(Bullet* bullet, int x, int y, int upgrade = 0);

void bullet_update(Bullet* bullet, int offset);

void bullet_draw(Bullet* bullet, SDL_Surface* screen, int color);

void bullet_kill(Bullet* bullet);

void upgrade_create(Upgrade* entity, int x, int y, int type);

void upgrade_draw(Upgrade* entity, SDL_Surface* screen, SDL_Surface* txt);

void upgrade_update(Upgrade* entity, int offset);

void upgrade_kill(Upgrade* entity);

void gun_init(Gun* gun);

void gun_reset(Gun* gun);

void gun_shoot(Gun* gun, int start_x, int start_y, int tick);

void gun_bullets_update(Gun* gun, int offset);

bool gun_bullet_collision_check(Gun* gun, SDL_Rect* hitbox);

void gun_upgrade_collision_check(Gun* gun, SDL_Rect* hitbox);

void gun_draw(Gun* gun, SDL_Surface* screen, int bulletcolor, SDL_Surface* upgrade_txt);