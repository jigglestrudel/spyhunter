#include "board.h"
#include "drawing_functions.h"

int get_width_at_y(Board board, int y)
{
	int index = (y + board.delta_height) / board.tile_height;
	return board.road_width[index];
}

void draw_board(Board board, SDL_Surface* screen, Uint32 grass_color, Uint32 road_color)
{
	SDL_FillRect(screen, NULL, grass_color);
	SDL_Rect rect;
	int drawable_tiles = screen->h / board.tile_height + 1;
	int bottom_margin = (int)(board.delta_height / board.tile_height) * board.tile_height - board.delta_height;
	for (int i = 0; i < drawable_tiles; i++)
	{
		rect = {100, (board.tile_height * i) + bottom_margin,
				get_width_at_y(board, (board.tile_height * i) + bottom_margin),
				board.tile_height};

		SDL_FillRect(screen, &rect, road_color);
	}
}
