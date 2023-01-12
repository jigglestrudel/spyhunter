#include <stdio.h>
#include <string.h>
#include "spyhunter.h"



int main(int argc, char **argv) {

	Game* game = nullptr;

	game = (Game*)malloc(sizeof(Game));
	if (!game)
	{
		printf("Memory error\n");
		return 1;
	}
	if (game_init(game))
	{
		printf("Init error\n");
		game_quit(game);
		return 1;
	}

	game_run(game);

	game_quit(game);
	return 0;
	};
