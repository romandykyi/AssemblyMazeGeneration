#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "maze_generation.h"

#define MIN_WIDTH 3
#define MIN_HEIGHT 3
#define MAX_WIDTH 100
#define MAX_HEIGHT 100

void printMaze(uint8_t* const maze, const uint8_t width, const uint8_t height)
{
	printf("  _");
	for (size_t j = 1; j < width; j++)
	{
		printf("__");
	}
	printf("\n");
	for (size_t i = 0; i < height; i++)
	{
		printf("|");
		for (size_t j = 0; j < width; j++)
		{
			if (i == height - 1 && j == width - 1)
			{
				printf(" |");
				break;
			}
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

int main(int argc, char** argv)
{
	if (argc != 3 && argc != 4)
	{
		fprintf(stderr, "Usage: '%s [width: %d-%d] [height: %d-%d] [optional seed]'\n", argv[0],
			MIN_WIDTH, MAX_WIDTH, MIN_HEIGHT, MAX_HEIGHT);
		return 1;
	}
	

	int width = atoi(argv[1]);
	if (width < MIN_WIDTH || width > MAX_WIDTH)
	{
		fprintf(stderr, "Invalid width\n");
		return 1;
	}
	int height = atoi(argv[2]);
	if (height < MIN_HEIGHT || height > MAX_HEIGHT)
	{
		fprintf(stderr, "Invalid height\n");
		return 1;
	}

	srand(argc == 4 ? atoi(argv[3]) : time(NULL));

	size_t mazeSize = width * height * sizeof(uint8_t);
	uint8_t* maze = (uint8_t*)malloc(mazeSize);

	printf("\n");
	printf("Binary tree:\n");
	memset(maze, BOTH_WALLS, mazeSize);
	generateMazeBinaryTree(maze, width, height);
	printMaze(maze, width, height);
	
	printf("\nSidewinder:\n");
	memset(maze, BOTH_WALLS, mazeSize);
	generateMazeSidewinder(maze, width, height);
	printMaze(maze, width, height);
	
	printf("\nRandomized DFS:\n");
	memset(maze, BOTH_WALLS, mazeSize);
	generateMazeDfs(maze, width, height);
	printMaze(maze, width, height);

	free(maze);
	return 0;
}
