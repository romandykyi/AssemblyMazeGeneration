#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "maze_generation.h"

#define MAZE_WIDTH 15
#define MAZE_HEIGHT 15

void print_maze(uint8_t* const maze, const uint8_t width, const uint8_t height)
{
	printf("_");
	for (size_t j = 0; j < width; j++)
	{
		printf("__");
	}
	printf("\n");
	for (size_t i = 0; i < height; i++)
	{
		printf("|");
		for (size_t j = 0; j < width; j++)
		{
			switch (maze[i * width + j])
			{
			case BOTH_WALLS:
				printf("_|");
				break;
			case RIGHT_PASSAGE:
				printf("__");
				break;
			case BOTTOM_PASSAGE:
				printf(" |");
				break;
			case NO_WALLS:
				printf("  ");
				break;
			}
		}
		printf("\n");
	}
}

int main()
{
	srand(time(NULL));

	size_t maze_size = MAZE_WIDTH * MAZE_HEIGHT * sizeof(uint8_t);
	uint8_t* maze = (uint8_t*)malloc(maze_size);
	
	printf("Sidewinder:\n");
	memset(maze, BOTH_WALLS, maze_size);
	generate_maze_sidewinder(maze, MAZE_WIDTH, MAZE_HEIGHT);
	print_maze(maze, MAZE_WIDTH, MAZE_HEIGHT);

	printf("\n");
	printf("Binary tree:\n");
	memset(maze, BOTH_WALLS, maze_size);
	generate_maze_binary_tree(maze, MAZE_WIDTH, MAZE_HEIGHT);
	print_maze(maze, MAZE_WIDTH, MAZE_HEIGHT);

	free(maze);
	system("pause");
	return 0;
}
