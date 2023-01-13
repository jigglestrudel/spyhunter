#include <stdio.h>
#include <string.h>
#include "spyhunter.h"



int main(int argc, char **argv) {

	Game_t* game = (Game_t*)malloc(sizeof(Game_t));
	if (!game)
		return 1;

	if (game_init(game))
	{
		printf("Init error\n");
		game_quit(game);
		return 1;
	}

	// main(0, nullptr);

	while (game->running)
	{
		game_handle_events(game);
		game_time_update(game);
		game_logic_update(game);
		game_screen_update(game);
	}

	game_quit(game);
	return 0;
	};
