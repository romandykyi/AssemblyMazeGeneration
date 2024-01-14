#pragma once
#include "maze_defs.h"
#include <stdlib.h>
#include <stdint.h>

void generateMazeBinaryTree(uint8_t* const maze, const uint32_t width, const uint32_t height);

void generateMazeSidewinder(uint8_t* const maze, const uint32_t width, const uint32_t height);

void generateMazeDfs(uint8_t* const maze, const uint32_t width, const uint32_t height);
