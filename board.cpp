#include "board.h"

int board_init(Board* board)
{
	board->delta_height = 0;
	board->tile_height = 100;
	board->road_width_length = 10;
	board->road_width = (int*)malloc(sizeof(int) * board->road_width_length);
	if (!board->road_width)
	{
		printf("Board allocation error\n");
		return 1;
	}

	for (int i = 0; i < board->road_width_length; i++)
	{
		board->road_width[i] = 20 * i + 200;
	}

	return 0;
}

int board_get_width(Board *board, int y)
{
	int index = ((y + board->delta_height) / board->tile_height) % board->road_width_length;
	return board->road_width[index];
}

void board_draw(Board* board, SDL_Surface* screen, Uint32 grass_color, Uint32 road_color)
{
	SDL_FillRect(screen, NULL, grass_color);
	SDL_Rect rect;
	int tile_width;
	int drawable_tiles = screen->h / board->tile_height + 2;
	int bottom_margin = (int)(board->delta_height / board->tile_height) * board->tile_height - board->delta_height;
	for (int i = 0; i < drawable_tiles; i++)
	{
		tile_width = board_get_width(board, (board->tile_height * i) + bottom_margin);
		rect = { (screen->w - tile_width) / 2, (board->tile_height * i) + bottom_margin,
				tile_width, board->tile_height};

		SDL_FillRect(screen, &rect, road_color);
	}
}

void board_move(Board* board, int d_y)
{
	board->delta_height -= d_y;
	if (board->delta_height < 0)
	{
		board->delta_height += board->road_width_length * board->tile_height;
	}
}

void board_kill(Board* board)
{
	if (board)
		free(board->road_width);
	free(board);
}
