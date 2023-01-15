#include "board.h"

int board_init(Board* board)
{
	board->grass_marigin = 0;
	board->skipped = 0;
	board->delta_height = 0;
	board->tile_height = ROAD_TILE_HEIGHT;
	
	

	if (road_load_file(board, "./road.txt") != 0)
	{
		printf("Board loading error\n");
		return 1;
	}

	return 0;
}

int road_load_file(Board* board, const char* path)
{
	int x;
	FILE* f = fopen(path, "r");
	if (f == NULL)
		return 1;

	fscanf(f, "%d", &(board->road_width_length));

	board->road_width = (int*)malloc(sizeof(int) * board->road_width_length);

	if (!board->road_width)
	{
		printf("Board allocation error\n");
		return 1;
	}

	for (int i = 0; i < board->road_width_length; i++)
	{
		fscanf(f, "%d", &(board->road_width[i]));
	}

	fclose(f);
}



int board_get_width(Board *board, int y)
{
	int index = ((board->delta_height - y) / ROAD_TILE_HEIGHT) % board->road_width_length;
	if (index < 0) index += board->road_width_length;
	return board->road_width[index];
}

int board_get_left_edge(Board_t* board, int y)
{
	return (SCREEN_WIDTH - board_get_width(board, y)) / 2;
}

int board_get_right_edge(Board_t* board, int y)
{
	return (SCREEN_WIDTH + board_get_width(board, y)) / 2;
}

void draw_grass(SDL_Surface* screen, SDL_Surface* grass, int offset)
{
	SDL_Rect rect, dst_rect;
	rect = {0, 0, grass->w, grass->h - offset };
	dst_rect = {0, offset, grass->w, grass->h - offset };
	SDL_BlitSurface(grass, &rect, screen, &dst_rect);
	rect = {0, grass->h - offset, grass->w, offset };
	dst_rect = {0, 0, grass->w, offset };
	SDL_BlitSurface(grass, &rect, screen, &dst_rect);
}

void board_draw(Board* board, SDL_Surface* screen, SDL_Surface* grass, SDL_Surface* tree, Uint32 road_color)
{
	SDL_Rect rect, dst_rect;
	int tile_width, y;
	int drawable_tiles = SCREEN_HEIGHT / ROAD_TILE_HEIGHT;
	
	
	draw_grass(screen, grass, board->grass_marigin);

	for (int i = -1; i < drawable_tiles+1; i++)
	{
		y = (board->tile_height * i) + board->skipped;
		tile_width = board_get_width(board, y);


		dst_rect = {board_get_left_edge(board, y), y, tile_width, ROAD_TILE_HEIGHT };
		SDL_FillRect(screen, &dst_rect, road_color);

		DrawSurface(screen, tree, board_get_left_edge(board, y) - TREE_DISTANCE - tree->w, y + ROAD_TILE_HEIGHT/2);
		DrawSurface(screen, tree, board_get_right_edge(board, y) + TREE_DISTANCE, y + ROAD_TILE_HEIGHT / 2);
	}
}


void board_move(Board* board, int d_y)
{
	board->grass_marigin += d_y;
	if (board->grass_marigin > SCREEN_HEIGHT) board->grass_marigin -= SCREEN_HEIGHT;
	if (board->grass_marigin < 0) board->grass_marigin += SCREEN_HEIGHT;

	board->skipped += d_y;
	if (board->skipped > board->tile_height) board->skipped -= board->tile_height;
	if (board->skipped < 0) board->skipped += board->tile_height;

	board->delta_height += d_y;
	if (board->delta_height > board->road_width_length) board->delta_height -= board->road_width_length * board->tile_height;
	if (board->delta_height < 0) board->delta_height += board->road_width_length * board->tile_height;

}


void board_kill(Board* board)
{
	if (board)
		free(board->road_width);
	free(board);
}
