#include "spyhunter.h"


void color_init(Game_t* game)
{
	game->colorEnum->black = SDL_MapRGB(game->screen->format, 0x00, 0x00, 0x00);
	game->colorEnum->green = SDL_MapRGB(game->screen->format, 0x00, 0xFF, 0x00);
	game->colorEnum->red = SDL_MapRGB(game->screen->format, 0xFF, 0x00, 0x00);
	game->colorEnum->blue = SDL_MapRGB(game->screen->format, 0x00, 0x00, 0xFF);
	game->colorEnum->yellow = SDL_MapRGB(game->screen->format, 0xFF, 0xFF, 0x00);
	game->colorEnum->magenta = SDL_MapRGB(game->screen->format, 0xFF, 0x00, 0xFF);
	game->colorEnum->cyan = SDL_MapRGB(game->screen->format, 0x00, 0xFF, 0xFF);
	game->colorEnum->white = SDL_MapRGB(game->screen->format, 0xFF, 0xFF, 0xFF);
}

void game_quit(Game_t* game)
{
	texture_kill(game->textureEnum);
	board_kill(game->board);
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
	SDL_SetWindowTitle(game->window, "Spyhunter Tomasz Krêpa 193047");

	game->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	game->scrtex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);
	
	return 0;
}

int game_reset(Game_t* game)
{
	if (board_init(game->board) != 0)
		return 1;

	game->running = true;
	game->paused = false;
	game->gameOver = false;
	game->downArrow = false;
	game->upArrow = false;
	game->rightArrow = false;
	game->leftArrow = false;
	game->space = false;
	game->pointsStopped = false;

	game->lastGameUpdate = SDL_GetTicks();

	game->ticks = 0;
	game->startTime = 0;
	game->lastGivenPoints = 0;
	game->pointsStoppedTime = 0;
	game->timeElapsed = 0;

	game->score = 0;
	game->lives = STARTING_LIVES;
	game->pointsCounted = 0;

	if (car_init(game->mainCar, PLAYER1, SCREEN_WIDTH/2, MAIN_CAR_Y) != 0)
		return 1;

	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		if (car_init(game->npcCars+i, WASTED, SCREEN_WIDTH/4 + 50 * i, 500 * (i+1)) != 0)
			return 1;
	}
	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		if (car_init(game->enemyCars + i, ENEMY, SCREEN_WIDTH / 4 + 50 * i, 600 + 100 * i) != 0)
			return 1;
	}

	gun_init(game->main_gun);

	return 0;
}

int game_init(Game_t* game)
{
	if (game_sdl_init(game) != 0)
		return 1;

	game->textureEnum = (TextureEnum*)malloc(sizeof(TextureEnum));
	game->board	= (Board_t*)malloc(sizeof(Board_t));
	game->colorEnum	= (ColorEnum*)malloc(sizeof(ColorEnum));
	game->mainCar = (Car_t*)malloc(sizeof(Car_t));
	game->npcCars = (Car_t*)malloc(sizeof(Car_t)*NPC_AMMOUNT);
	game->enemyCars = (Car_t*)malloc(sizeof(Car_t)*ENEMY_AMMOUNT);
	game->main_gun = (Gun*)malloc(sizeof(Gun));

	color_init(game);
	if (texture_load(game->textureEnum) != 0)
		return 1;

	game_reset(game);


	return 0;
}

void game_gui_draw(Game_t* game)
{
	char str[64];

	DrawString(game->screen, 2, 2, "Spy Hunter Tomasz Krepa 193047", game->textureEnum->charset, CHARSET_SIZE);

	sprintf(str, "%s", "SCORE");
	DrawStringCentered(game->screen, SCREEN_WIDTH / 4, 30, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);

	if (game->pointsStopped)
	{
		sprintf(str, "%s", "NO POINTS");
		DrawStringCentered(game->screen, SCREEN_WIDTH / 4, 30 + BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);
	}
	else
	{
		sprintf(str, "%llu", game->score);
		DrawStringCentered(game->screen, SCREEN_WIDTH / 4 , 30+BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);
	}
	
	sprintf(str, "%d", (int)((game->timeElapsed) * 0.001));
	DrawStringCentered(game->screen, SCREEN_WIDTH / 2, 30+BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);

	sprintf(str, "%s", "CARS");
	DrawStringCentered(game->screen, 3*SCREEN_WIDTH / 4, 30, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);

	if ((int)((game->timeElapsed) * 0.001) < UNLIMITED_LIVES_PERIOD)
	{
		sprintf(str, "%s", "UNLIMITED");
		DrawStringCentered(game->screen, 3 * SCREEN_WIDTH / 4, 30 + BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);
	}
	else
	{
		sprintf(str, "%d", game->lives);
		DrawStringCentered(game->screen, 3 * SCREEN_WIDTH / 4, 30 + BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);
	}

	if (game->main_gun->ammo)
	{
		sprintf(str, "%s", "BIG AMMO");
		DrawStringCentered(game->screen, 3 * SCREEN_WIDTH / 4, 30 + 3*BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);
		sprintf(str, "%d", game->main_gun->ammo);
		DrawStringCentered(game->screen, 3 * SCREEN_WIDTH / 4, 30 + 4*BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);
	}
	else
	{
		sprintf(str, "%s", "AMMO");
		DrawStringCentered(game->screen, 3 * SCREEN_WIDTH / 4, 30 + 3 * BIGCHARSET_SIZE, str, game->textureEnum->bigcharset, BIGCHARSET_SIZE);
		DrawStringCentered(game->screen, 3 * SCREEN_WIDTH / 4, 30 + 4 * BIGCHARSET_SIZE, "inf", game->textureEnum->bigcharset, BIGCHARSET_SIZE);

	}

	DrawString(game->screen, SCREEN_WIDTH - strlen(FINISHED_REQUIERMENTS)*CHARSET_SIZE, SCREEN_HEIGHT-CHARSET_SIZE, FINISHED_REQUIERMENTS, game->textureEnum->charset, CHARSET_SIZE);
}

void game_over_draw(Game_t* game)
{
	char str[64];

	//DrawRectangle(game->screen, (SCREEN_WIDTH - BOX_WIDTH) / 2, (SCREEN_HEIGHT - BOX_HEIGHT) / 2, BOX_WIDTH, BOX_HEIGHT, game->colorEnum->magenta, game->colorEnum->blue);
	DrawStringCentered(game->screen, SCREEN_WIDTH / 2, (SCREEN_HEIGHT - BOX_HEIGHT) / 2 + CHARSET_SIZE,
						"GAME OVER", game->textureEnum->bigcharset, BIGCHARSET_SIZE);
	DrawStringCentered(game->screen, SCREEN_WIDTH/2, (SCREEN_HEIGHT - BOX_HEIGHT) / 2 + 2*BIGCHARSET_SIZE,
						"press [n] to start a new game", game->textureEnum->charset, CHARSET_SIZE);
}

void game_paused_draw(Game_t* game)
{
	char str[64];

	//DrawRectangle(game->screen, (SCREEN_WIDTH - BOX_WIDTH) / 2, (SCREEN_HEIGHT - BOX_HEIGHT) / 2, BOX_WIDTH, BOX_HEIGHT, game->colorEnum->magenta, game->colorEnum->blue);
	DrawStringCentered(game->screen, SCREEN_WIDTH / 2, (SCREEN_HEIGHT - BOX_HEIGHT) / 2 + CHARSET_SIZE,
		"game paused", game->textureEnum->bigcharset, BIGCHARSET_SIZE);
	DrawStringCentered(game->screen, SCREEN_WIDTH / 2, (SCREEN_HEIGHT - BOX_HEIGHT) / 2 + 2 * BIGCHARSET_SIZE,
		"press [p] to unpause", game->textureEnum->charset, CHARSET_SIZE);
}

void game_draw_other_cars(Game_t* game, SDL_Surface* screen, TextureEnum* textureEnum)
{
	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		car_draw(game->enemyCars + i, screen, textureEnum);
	}
	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		car_draw(game->npcCars + i, screen, textureEnum);
	}
}

void game_screen_update(Game_t* game)
{
	SDL_FillRect(game->screen, NULL, game->colorEnum->black);

	board_draw(game->board, game->screen, game->textureEnum->grass, game->textureEnum->tree, game->colorEnum->black, game->colorEnum->yellow);

	game_draw_other_cars(game, game->screen, game->textureEnum);

	car_draw(game->mainCar, game->screen, game->textureEnum);

	gun_draw(game->main_gun, game->screen, game->colorEnum->white, game->textureEnum->upgrade);

	game_gui_draw(game);

	if (game->gameOver)
		game_over_draw(game);
	else if (game->paused)
		game_paused_draw(game);

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
			else if (event.key.keysym.sym == SDLK_LEFT)
				game->leftArrow = true;
			else if (event.key.keysym.sym == SDLK_RIGHT)
				game->rightArrow = true;
			else if (event.key.keysym.sym == SDLK_SPACE)
				game->space = true;
			else if (event.key.keysym.sym == SDLK_n)
			{
				game_reset(game);
			}
			else if (event.key.keysym.sym == SDLK_p)
			{
				game->paused = !game->paused;
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_DOWN)
				game->downArrow = false;
			else if (event.key.keysym.sym == SDLK_UP)
				game->upArrow = false;
			else if (event.key.keysym.sym == SDLK_LEFT)
				game->leftArrow = false;
			else if (event.key.keysym.sym == SDLK_RIGHT)
				game->rightArrow = false;
			else if (event.key.keysym.sym == SDLK_SPACE)
				game->space = false;
			break;

		default:
			break;
		}
	}
}

void game_maincar_speed_update(Game_t* game)
{
	if (game->upArrow)
	{
		car_speed_up(game->mainCar);
	}
	else if (game->downArrow)
	{
		if (game->mainCar->speed > 0)
		{
			car_hit_break(game->mainCar);
		}
		else
		{
			car_speed_up_reverse(game->mainCar);
		}
	}
	else
	{	
		car_slow_down(game->mainCar);
	}
	
}

void game_maincar_turn_update(Game_t* game)
{
	if (game->leftArrow)
		car_turn_left(game->mainCar);
	else if (game->rightArrow)
		car_turn_right(game->mainCar);
	else
		car_go_straight(game->mainCar);
}

void game_maincar_add_points(Game_t* game)
{
	if (car_road_state(game->mainCar, game->board) == ROAD && game->mainCar->speed > 5 && game->ticks - game->lastGivenPoints > POINT_FOR_DRIVING_INTERVAL && !game->pointsStopped)
	{
		game->lastGivenPoints = game->ticks;
		game->score += POINTS_FOR_DRIVING;
		if (game->score - game->pointsCounted > LIVE_POINT_COST)
		{
			game->pointsCounted += LIVE_POINT_COST;
			game->lives++;
		}
	}

}


void game_maincar_stat_update(Game_t* game)
{
	game_maincar_speed_update(game);
	game_maincar_turn_update(game);

	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		if ((game->enemyCars + i)->carType != WASTED)
			car_check_collision(game->mainCar, game->enemyCars + i);
	}
	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		if ((game->npcCars + i)->carType != WASTED)
			car_check_collision(game->mainCar, game->npcCars + i);
	}
}

void game_maincar_update(Game_t* game)
{
	game_maincar_stat_update(game);

	if (game->mainCar->carType != WASTED) 
	{
		car_hitbox_update(game->mainCar, game->mainCar->speed, game->board);
		game_maincar_add_points(game);
	}
	else
	{
		game->mainCar->speed = 0;
		game->mainCar->turn = 0;
		if (game->mainCar->deathTime == 0)
		{
			if ((int)((game->timeElapsed) * 0.001) < UNLIMITED_LIVES_PERIOD || --(game->lives) > 0)
				game->mainCar->deathTime = game->ticks;
			else
				game->gameOver = true;
		}
		else if (game->ticks - game->mainCar->deathTime > MAIN_RESPAWN_TIME)
		{
			car_respawn(game->mainCar, PLAYER1, MAIN_CAR_X, MAIN_CAR_Y);
		}
	}
}

void game_other_cars_stat_update(Game_t* game)
{
	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		car_ai(game->enemyCars + i, game->mainCar);
		for (int j = 0; j < ENEMY_AMMOUNT; j++)
		{
			car_ai(game->enemyCars + i, game->enemyCars + j);
		}
		for (int j = 0; j < NPC_AMMOUNT; j++)
		{
			car_ai(game->enemyCars + i, game->npcCars + j);
		}

		if ((game->enemyCars + i)->carType != WASTED)
		{
			for (int j = 0; j < NPC_AMMOUNT; j++)
			{
				car_check_collision(game->enemyCars + i, game->npcCars + j);
			}
			for (int j = 0; j < ENEMY_AMMOUNT; j++)
			{
				car_check_collision(game->enemyCars + i, game->enemyCars + j);
			}
		}
	}

	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		car_ai(game->npcCars + i, game->mainCar);

	}
}
void game_other_cars_update(Game_t* game)
{
	game_other_cars_stat_update(game);

	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		car_update(game->enemyCars + i, game->mainCar->speed, game->board);
	}
	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		car_update(game->npcCars + i, game->mainCar->speed, game->board);
	}

	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		if ((game->enemyCars + i)->deathTime == 0 && (game->enemyCars + i)->carType == WASTED)
		{
			(game->enemyCars + i)->deathTime = game->ticks;
			if ((game->enemyCars + i)->lastTouchedBy == PLAYER1 && !game->pointsStopped)
				game->score += SCORE_FOR_ENEMY_DESTRUCTION;
		}
	}
	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		if ((game->npcCars + i)->deathTime == 0 && (game->npcCars + i)->carType == WASTED)
		{
			(game->npcCars + i)->deathTime = game->ticks;
			if ((game->npcCars + i)->lastTouchedBy == PLAYER1)
			{
				game->pointsStopped = true;
				game->pointsStoppedTime = game->ticks;
			}
		}
	}

}

void game_respawn_cars(Game_t* game)
{

	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		if (car_can_respawn(game->enemyCars + i, game->ticks))
		{
			car_respawn(game->enemyCars + i, ENEMY, SCREEN_WIDTH/4 + (i%SPAWN_CAR_PER_ROW)*SPAWN_GAP, SCREEN_HEIGHT * 1.5 * ((i % 2) ? 1 : -1) + SPAWN_GAP*(int)(i/SPAWN_CAR_PER_ROW), MAX_ENEMY_SPEED);
		}

	}
	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		if (car_can_respawn(game->npcCars + i, game->ticks))
		{
			car_respawn(game->npcCars + i, NPC, 200 + i * 60 * ((i % 2) ? 0.5 : 1), SCREEN_HEIGHT * ((i%2) ? -1 : 1), MAX_NPC_SPEED - i * ((i % 2) ? 1 : 2));
			
		}
	}
}

void game_gun_update(Game_t* game)
{
	if (game->space && game->mainCar->carType != WASTED)
	{
		gun_shoot(game->main_gun, car_get_middle_x(game->mainCar), game->mainCar->hitBox->y, game->ticks);
	}

	gun_bullets_update(game->main_gun, game->mainCar->speed);
	upgrade_update(game->main_gun->upgrade_entity, game->mainCar->speed);

	for (int i = 0; i < ENEMY_AMMOUNT; i++)
	{
		if (gun_bullet_collision_check(game->main_gun, (game->enemyCars + i)->hitBox))
		{
			(game->enemyCars + i)->carType = WASTED;
			(game->enemyCars + i)->lastTouchedBy = PLAYER1;
		}
		
	}
	for (int i = 0; i < NPC_AMMOUNT; i++)
	{
		if (gun_bullet_collision_check(game->main_gun, (game->npcCars + i)->hitBox))
		{
			(game->npcCars + i)->carType = WASTED;
			(game->npcCars + i)->lastTouchedBy = PLAYER1;
		}
	}

	gun_upgrade_collision_check(game->main_gun, game->mainCar->hitBox);
	
}


void game_logic_update(Game_t* game)
{
	if (SDL_GetTicks() - game->lastGameUpdate > TICK_UPDATE_CAP)
	{
		game->lastGameUpdate = SDL_GetTicks();
		if(!game->paused && !game->gameOver)
		{
			game->ticks += TICK_UPDATE_CAP;

			if (game->ticks - game->pointsStoppedTime > POINT_COOLDOWN_FOR_NPC_DESTRUCTION)
			{
				game->pointsStopped = false;
			}

			game->timeElapsed = game->ticks - game->startTime;
			game_gun_update(game);
			game_maincar_update(game);
			game_other_cars_update(game);
			board_move(game->board, game->mainCar->speed);
			game_respawn_cars(game);
		}
	}
}

void game_run(Game_t* game)
{
	while (game->running)
	{
		game_handle_events(game);
		game_logic_update(game);
		game_screen_update(game);
	}
}

