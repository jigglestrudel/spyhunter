#include "spyhunter.h"


void color_init(Game_t* game)
{
	game->colorEnum->black = SDL_MapRGB(game->screen->format, 0x00, 0x00, 0x00);
	game->colorEnum->green = SDL_MapRGB(game->screen->format, 0x00, 0xFF, 0x00);
	game->colorEnum->red = SDL_MapRGB(game->screen->format, 0xFF, 0x00, 0x00);
	game->colorEnum->blue = SDL_MapRGB(game->screen->format, 0x11, 0x11, 0xDD);
}

int load_one_texture(SDL_Surface** texture, const char* path)
{
	*(texture) = SDL_LoadBMP(path);
	if (*(texture) == NULL) {
		printf("Texture %s error: %s\n", path, SDL_GetError());
		return 1;
	}
	return 0;
}

int texture_load(Game_t* game)
{
	load_one_texture(&(game->textureEnum->car_npc), "./car_npc.bmp");
	load_one_texture(&(game->textureEnum->car_enemy), "./car_enemy.bmp");
	load_one_texture(&(game->textureEnum->car_player), "./car_player.bmp");
	load_one_texture(&(game->textureEnum->car_player2), "./car_player2.bmp");
	return 0;
}

void game_quit(Game_t* game)
{
	board_kill(game->board);
	SDL_FreeSurface(game->charset);
	SDL_FreeSurface(game->screen);
	SDL_DestroyTexture(game->scrtex);
	SDL_DestroyWindow(game->window);
	SDL_DestroyRenderer(game->renderer);
	SDL_Quit();
}

int game_sdl_init(Game_t* game)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &(game->window), &(game->renderer)) != 0)
	{
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(game->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(game->window, "Spyhunter");

	game->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	game->scrtex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);
	
	return 0;
}

int game_init(Game_t* game)
{
	if (game_sdl_init(game) != 0)
		return 1;

	if (load_one_texture( &(game->charset), "./cs8x8.bmp") != 0)
		return 1;
	SDL_SetColorKey(game->charset, true, 0x000000);

	game->textureEnum = (TextureEnum*)malloc(sizeof(TextureEnum));
	game->board	= (Board_t*)malloc(sizeof(Board_t));
	game->colorEnum	= (ColorEnum*)malloc(sizeof(ColorEnum));
	game->main_car = (Car_t*)malloc(sizeof(Car_t));

	game->ticks = SDL_GetTicks();
	game->frames = 0;
	game->running = true;
	game->worldTime = 0;
	game->downArrow = false;
	game->upArrow = false;
	game->globalSpeed = 0;
	game->speedT1 = SDL_GetTicks();
	game->lastGameUpdate = SDL_GetTicks();

	color_init(game);
	texture_load(game);
	if (board_init(game->board) != 0)
		return 1;
	if (car_init(game->main_car, PLAYER1) != 0)
		return 1;

	return 0;
}

void game_time_update(Game_t* game)
{
	int t2 = SDL_GetTicks();

	double delta = (t2 - game->ticks) * 0.001;
	game->ticks = t2;

	game->worldTime += delta;
}

void game_screen_update(Game_t* game)
{
	SDL_FillRect(game->screen, NULL, game->colorEnum->black);

	board_draw(game->board, game->screen, game->colorEnum->green, game->colorEnum->black);
	car_draw(game->main_car, game->screen, game->textureEnum);
	DrawString(game->screen, 0, 0, "Spy Hunter 193047", game->charset);

	SDL_UpdateTexture(game->scrtex, NULL, game->screen->pixels, game->screen->pitch);
	SDL_RenderCopy(game->renderer, game->scrtex, NULL, NULL);
	SDL_RenderPresent(game->renderer);
}

void game_handle_events(Game_t* game)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				game->running = false;

			else if (event.key.keysym.sym == SDLK_DOWN)
				game->downArrow = true;
			else if (event.key.keysym.sym == SDLK_UP)
				game->upArrow = true;
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_DOWN)
				game->downArrow = false;
			else if (event.key.keysym.sym == SDLK_UP)
				game->upArrow = false;
			break;

		default:
			break;
		}
	}
}

void game_speed_update(Game_t* game)
{
	if (game->upArrow)
	{
		if (game->globalSpeed == ZERO)
		{
			printf("START");
			game->globalSpeed = SLOW;
			game->speedT1 = game->ticks;
		}
		else if (game->globalSpeed > ZERO && game->ticks - game->speedT1 > 150)
		{
			printf("PRZYSPIESZA\n");
			game->speedT1 = game->ticks;

			switch (game->globalSpeed)
			{
			case SLOW:
				game->globalSpeed = MEDIUM;
				break;
			case MEDIUM:
				game->globalSpeed = FAST;
				break;
			default:
				break;
			}
		}
	}
	else if (game->downArrow)
	{
		if (game->globalSpeed == ZERO)
		{
			game->globalSpeed = REVERSE;
		}
		else if (game->globalSpeed > ZERO && game->ticks - game->speedT1 > 70)
		{
			printf("HAMUJE\n");
			game->speedT1 = game->ticks;

			switch (game->globalSpeed)
			{
			case SLOW:
				game->globalSpeed = ZERO;
				break;
			case MEDIUM:
				game->globalSpeed = SLOW;
				break;
			case FAST:
				game->globalSpeed = MEDIUM;
				break;
			default:
				break;
			}
		}
	}
	else 
	{
		
		if (game->globalSpeed > ZERO)
		{
			printf("ZWALNIA");

			switch (game->globalSpeed)
			{
			case SLOW:
				game->globalSpeed = ZERO;
				break;
			case MEDIUM:
				game->globalSpeed = SLOW;
				break;
			case FAST:
				game->globalSpeed = MEDIUM;
				break;
			default:
				break;
			}
		}
	}


}

void game_logic_update(Game_t* game)
{
	game->ticks = SDL_GetTicks();
	if (game->ticks - game->lastGameUpdate > 25)
	{
		game_speed_update(game);
		board_move(game->board, game->globalSpeed);
		game->lastGameUpdate = game->ticks;

	}
}

