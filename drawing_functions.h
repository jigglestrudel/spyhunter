#pragma once
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "./SDL2-2.0.10/include/SDL.h"

typedef struct ColorEnum
{
	Uint32 black;
	Uint32 green;
	Uint32 red;
	Uint32 blue;
	Uint32 yellow;
	Uint32 magenta;
	Uint32 cyan;
	Uint32 white;
	

} ColorEnum;

typedef struct TextureEnum
{
	SDL_Surface* car_npc;
	SDL_Surface* car_enemy;
	SDL_Surface* car_player;
	SDL_Surface* car_player2;
	SDL_Surface* tree;
	SDL_Surface* bullet;
	SDL_Surface* gun;
	SDL_Surface* eti;
	SDL_Surface* charset;
	SDL_Surface* bigcharset;
	SDL_Surface* grass;
	SDL_Surface* exploded_car;

}TextureEnum;

void DrawString(SDL_Surface* screen, int x, int y, const char* text,SDL_Surface* charset, int charset_size);

void DrawStringCentered(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset, int charset_size);


void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

void texture_kill(TextureEnum* txtenum);

int load_one_texture(SDL_Surface** texture, const char* path);

int texture_load(TextureEnum* txtenum);

