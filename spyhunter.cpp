#include "spyhunter.h"


void color_init(Game* game)
{
	game->colorEnum->black = SDL_MapRGB(game->screen->format, 0x00, 0x00, 0x00);
	game->colorEnum->green = SDL_MapRGB(game->screen->format, 0x00, 0xFF, 0x00);
	game->colorEnum->red = SDL_MapRGB(game->screen->format, 0xFF, 0x00, 0x00);
	game->colorEnum->blue = SDL_MapRGB(game->screen->format, 0x11, 0x11, 0xCC);
}

void game_quit(Game* game)
{
	SDL_FreeSurface(game->charset);
	SDL_FreeSurface(game->screen);
	SDL_DestroyTexture(game->scrtex);
	SDL_DestroyWindow(game->window);
	SDL_DestroyRenderer(game->renderer);
	SDL_Quit();
}


int game_init(Game* game)
{

	game->colorEnum = (ColorEnum*)malloc(sizeof(ColorEnum));
	game->t1 = SDL_GetTicks();
	game->frames = 0;
	game->running = true;
	game->worldTime = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&(game->window), &(game->renderer));

	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	}

	game->charset = SDL_LoadBMP("./cs8x8.bmp");
	if (game->charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		return 1;
	};
	SDL_SetColorKey(game->charset, true, 0x000000);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(game->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(game->window, "Spyhunter");

	game->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	game->scrtex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	color_init(game);

	return 0;
}

void game_time_update(Game* game)
{
	int t2 = SDL_GetTicks();

	double delta = (t2 - game->t1) * 0.001;
	game->t1 = t2;

	game->worldTime += delta;
}

void game_screen_update(Game* game)
{
	SDL_FillRect(game->screen, NULL, game->colorEnum->black);

	DrawString(game->screen, 0, 0, "Spy Hunter 193047", game->charset);

	SDL_UpdateTexture(game->scrtex, NULL, game->screen->pixels, game->screen->pitch);
	SDL_RenderCopy(game->renderer, game->scrtex, NULL, NULL);
	SDL_RenderPresent(game->renderer);
}

void game_handle_events(Game* game)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) game->running = false;


		default:
			break;
		}
	}
}

void game_run(Game* game)
{
	while (game->running)
	{
		game_time_update(game);
		game_screen_update(game);

		game_handle_events(game);
	}
}
