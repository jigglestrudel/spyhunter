#include "drawing_functions.h"

int load_one_texture(SDL_Surface** texture, const char* path)
{
	*(texture) = SDL_LoadBMP(path);
	if (*(texture) == NULL) {
		return 1;
	}
	return 0;
}

int texture_load(TextureEnum* textureEnum)
{
	load_one_texture(&(textureEnum->car_npc), "./car_npc.bmp");
	SDL_SetColorKey(textureEnum->car_npc, true, 0x000000);

	load_one_texture(&(textureEnum->car_enemy), "./car_spy.bmp");
	SDL_SetColorKey(textureEnum->car_enemy, true, 0x000000);

	load_one_texture(&(textureEnum->car_player), "./car_player.bmp");
	SDL_SetColorKey(textureEnum->car_player, true, 0x000000);

	load_one_texture(&(textureEnum->car_player2), "./car_player2.bmp");
	SDL_SetColorKey(textureEnum->car_player2, true, 0x000000);

	load_one_texture(&(textureEnum->charset), "./cs8x8.bmp");
	SDL_SetColorKey(textureEnum->charset, true, 0x000000);

	load_one_texture(&(textureEnum->bigcharset), "./cs16x16.bmp");
	SDL_SetColorKey(textureEnum->bigcharset, true, 0x000000);

	load_one_texture(&(textureEnum->tree), "./tree.bmp");
	SDL_SetColorKey(textureEnum->tree, true, 0x000000);

	load_one_texture(&(textureEnum->exploded_car), "./exploded_car.bmp");
	SDL_SetColorKey(textureEnum->exploded_car, true, 0x000000);

	load_one_texture(&(textureEnum->upgrade), "./upgrade.bmp");

	load_one_texture(&(textureEnum->eti), "./eti.bmp");

	load_one_texture(&(textureEnum->grass), "./grass.bmp");

	return 0;
}

// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset, int charset_size)
{
	int charx, chary, c;
	SDL_Rect src_rect, dest;
	src_rect.w = charset_size;
	src_rect.h = charset_size;
	dest.w = charset_size;
	dest.h = charset_size;
	while (*text) {
		if (*text == '\n')
		{
			y += charset_size;
			text++;
			continue;
		}
		c = *text & 255;
		charx = (c % 16) * charset_size;
		chary = (c / 16) * charset_size;
		src_rect.x = charx;
		src_rect.y = chary;
		dest.x = x;
		dest.y = y;
		SDL_BlitSurface(charset, &src_rect, screen, &dest);
		x += charset_size;
		text++;
	}
}

void DrawStringCentered(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset, int charset_size)
{
	DrawString(screen, x - strlen(text) * charset_size / 2, y, text, charset, charset_size);
}



// draw a surface sprite on a surface screen in point (x, y) top left
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}


// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
}


// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	}
}


// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);

	SDL_Rect rect = { x + 1, y + 1, l - 2, k - 2 };
	SDL_FillRect(screen, &rect, fillColor);

}

void texture_kill(TextureEnum* txtenum)
{
	SDL_FreeSurface(txtenum->car_npc);
	SDL_FreeSurface(txtenum->car_enemy);
	SDL_FreeSurface(txtenum->car_player);
	SDL_FreeSurface(txtenum->car_player2);
	SDL_FreeSurface(txtenum->tree);
	SDL_FreeSurface(txtenum->eti);
	SDL_FreeSurface(txtenum->charset);
	SDL_FreeSurface(txtenum->bigcharset);
	SDL_FreeSurface(txtenum->exploded_car);
	free(txtenum);
}

