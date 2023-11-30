#pragma once
#include "maze_defs.h"
#include <stdlib.h>
#include <stdint.h>

void generate_maze_binary_tree(uint8_t* const maze, const uint32_t width, const uint32_t height);

void generate_maze_sidewinder(uint8_t* const maze, const uint32_t width, const uint32_t height);

void generate_maze_dfs(uint8_t* maze, const uint32_t width, const uint32_t height);