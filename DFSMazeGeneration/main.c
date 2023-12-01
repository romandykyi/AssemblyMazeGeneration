#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "maze_generation.h"

#define MAZE_WIDTH 15
#define MAZE_HEIGHT 15

//void generate_maze(char maze[MAZE_SIZE])
//{
//	// Array that contains 1 for visited cells and 0 for unvisited cells
//	char visited[MAZE_SIZE];
//	memset(visited, UNVISITED, sizeof(char) * MAZE_SIZE);
//
//	uint32_t current_cell = 0;
//	uint32_t stack_size = 0;
//
//	__asm {
//		// Choose the initial cell(rand() % MAZE_SIZE)
//		call rand
//		mov ebx, MAZE_SIZE
//		xor edx, edx
//		div ebx // Remainder is in edx
//
//		// Mark the initial cell as visited
//		lea ebx, [visited]
//		add ebx, edx
//		mov[ebx], VISITED
//
//		// Push it to the stack
//		push edx
//		// Update size of the stack
//		mov stack_size, 1
//
//		traverse:
//		lea esi, [visited]
//
//		// Pop a cell from the stack and make it a current cell
//		pop ebx
//		mov current_cell, ebx
//		// Update size of the stack
//		dec stack_size
//
//		// Number of unvisited neighbors of the cell
//		xor ecx, ecx
//
//		// Get a remainder
//		mov eax, ebx
//		mov ebx, MAZE_WIDTH
//		xor edx, edx
//		div ebx
//
//		// Check left neighbor(if cell is not at the left edge)
//		test edx, edx
//		jz skip_left
//		// Get an index of a left neighbor
//		mov eax, ebx
//		dec eax
//		// Get a memory index
//		lea edi, [esi]
//		add edi, eax
//		// Check if it's unvisited
//		test [edi], VISITED
//		jnz skip_left
//		// Push it to a stack
//		push eax
//		inc ecx
//		skip_left:
//
//		// Check right neighbor(if cell is not at the left edge)
//		cmp edx, MAZE_WIDTH
//		jge skip_right
//		// Get an index of a left right
//		mov eax, ebx
//		inc eax
//		// Get a memory index
//		lea edi, [esi]
//		add edi, eax
//		// Check if it's unvisited
//		test [edi], VISITED
//		jnz skip_right
//		// Push it to a stack
//		push eax
//		inc ecx
//		skip_right:
//
//		// While the stack is not empty
//		mov eax, stack_size
//		test eax, eax
//		jnz traverse
//	}
//
//}

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
	memset(maze, BOTH_WALLS, maze_size);
	generate_maze_sidewinder(maze, MAZE_WIDTH, MAZE_HEIGHT);
	print_maze(maze, MAZE_WIDTH, MAZE_HEIGHT);

	memset(maze, BOTH_WALLS, maze_size);
	generate_maze_binary_tree(maze, MAZE_WIDTH, MAZE_HEIGHT);
	print_maze(maze, MAZE_WIDTH, MAZE_HEIGHT);

	free(maze);
	system("pause");
	return 0;
}
