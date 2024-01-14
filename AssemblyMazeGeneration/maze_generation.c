#include "maze_generation.h"

void generateMazeBinaryTree(uint8_t* const maze, const uint32_t width, const uint32_t height)
{
	__asm {
		mov edx, width
		// Counter(width - 1 to 0)
		mov ecx, edx
		dec ecx
		// Start index
		mov esi, maze
		// Index of the last cell without the last row
		mov edi, esi
		// width * (height - 1)
		mov eax, edx
		mov ebx, height
		sub ebx, 1
		mul ebx
		add edi, eax

		l1 :
			cmp ecx, 0
			je carve_bottom

			// Carve right or bottom passage
			push ecx // Save ecx
			// "Flip a coin"
			call rand
			and eax, 1 // Modulo of 2
			// Add 1 to make eax either RIGHT_PASSAGE(1) or BOTTOM_PASSAGE(2)
			inc eax
			mov [esi], eax
			pop ecx // Get ecx back
			jmp condition
			carve_bottom :
			mov [esi], BOTTOM_PASSAGE
			mov ecx, width

			condition :
			dec ecx
			inc esi
			cmp esi, edi
			jl l1

		// Carve right walls at the last row
		// Start index
		mov esi, edi
		// Index of the second from the end cell
		add edi, width
		dec edi
		l2 :
			mov [esi], RIGHT_PASSAGE
			inc esi
			cmp esi, edi
			jl l2
	}
}

void generateMazeSidewinder(uint8_t* const maze, const uint32_t width, const uint32_t height)
{
	__asm {
		mov edx, width
		// Counter(width - 1 to 0)
		mov ecx, width
		sub ecx, 1
		// Start index
		mov esi, maze
		// Index of the last cell
		mov edi, esi
		// width * height
		mov eax, edx
		mov ebx, height
		mul ebx
		add edi, eax

		// Carve right walls in the first row
		l1 :
			mov [esi], RIGHT_PASSAGE
			inc esi
			loop l1

		// Skip the last cell in the first row
		inc esi

		// Number of cells in the current run
		xor ebx, ebx
		// Counter(width - 1 to 0)
		mov ecx, width
		dec ecx

		l2 :
			// Add current cell to the run
			push esi
			inc ebx

			// Check if current cell is last in the row
			cmp ecx, 0
			je new_row

			// Save the counter before calling rand 
			push ecx
			// Get a random number
			call rand
			// Get the counter back from the stack
			pop ecx

			// Clear the run with 50% probability
			test eax, 1
			je clear_run
			// Or carve a right passage
			or [esi], RIGHT_PASSAGE
			jmp condition

			new_row :
			mov ecx, width
			clear_run :
			cmp ebx, 0
			je condition
			// Save the counter before calling rand 
			push ecx
			// Get a random number
			call rand
			// Get the counter back from the stack
			pop ecx
			// Random cell is at edx:
			xor edx, edx
			div ebx
			// A loop from ebx-1 to 0
			pop_cells :
			dec ebx
			// Pop a cell
			pop eax
			// If it's the selected cell
			cmp ebx, edx
			jne pop_cells_condition
			// Carve a bottom passage for the top cell
			sub eax, width
			or [eax], BOTTOM_PASSAGE

			pop_cells_condition :
			cmp ebx, 0
			jg pop_cells

			condition :
			dec ecx
			inc esi
			cmp esi, edi
			jl l2
	}
}

void generateMazeDfs(uint8_t* const maze, const uint32_t width, const uint32_t height)
{
	// Allocate an array for saving cells state(visited/unvisited)
	int mazeSize = width * height * sizeof(uint8_t);
	uint8_t* visited = (uint8_t*)malloc(mazeSize);
	memset(visited, UNVISITED, mazeSize);

	__asm {
		// EDI - State of cells
		mov edi, visited

		// Pick a random cell
		call rand
		// Perform modulo operation
		xor edx, edx
		mov ebx, mazeSize
		div ebx
		// Push random cell into stack
		push edx
		// And mark it as visited
		mov esi, edi
		add esi, edx
		mov [esi], VISITED
		// ECX - Size of a stack
		mov ecx, 1

		l:
		// Pop a cell from the stack
		pop ebx
		dec ecx
		// Preserve stack size
		push ecx
		// Preserve current cell
		push ebx
		mov eax, ebx

		// Find unvisited neighbors of a cell(ecx is a number of such cells)
		xor ecx, ecx
		xor edx, edx
		mov ebx, width
		// eax = row = currentCell / width
		// edx = column = currentCell % width
		div ebx
		// Get current cell back
		pop ebx

		// Get left neighbor
		dec ebx
		// Left neighbor exists if column != 0
		cmp edx, 0
		je skip_left
			// Check if is unvisited
			mov esi, edi
			add esi, ebx
			cmp [esi], VISITED
			je skip_left

			// Push left cell
			push LEFT_CELL
			inc ecx
		skip_left:
		// Return ebx to its previous state
		inc ebx

		// Get right neighbor
		inc ebx
		// Right neighbor exists if column != width - 1 -> column + 1 != width
		inc edx
		cmp edx, width
		je skip_right
			// Check if is unvisited
			mov esi, edi
			add esi, ebx
			cmp [esi], VISITED
			je skip_right

			// Push right cell
			push RIGHT_CELL
			inc ecx
		skip_right:
		// Return ebx to its previous state
		dec ebx

		// EDX is now free to use, so use it to save width
		mov edx, width

		// Get top neighbor
		sub ebx, edx
		// Top neighbor exists if row != 0
		cmp eax, 0
		je skip_top
			// Check if is unvisited
			mov esi, edi
			add esi, ebx
			cmp [esi], VISITED
			je skip_top

			// Push top cell
			push TOP_CELL
			inc ecx
		skip_top:
		// Return ebx to its previous state
		add ebx, edx

		// Get bottom neighbor
		add ebx, edx
		// Bottom neighbor exists if row != height - 1 -> row + 1 != height
		inc eax
		cmp eax, height
		je skip_bottom
			// Check if is unvisited
			mov esi, edi
			add esi, ebx
			cmp [esi], VISITED
			je skip_bottom

			// Push bottom cell
			push BOTTOM_CELL
			inc ecx
		skip_bottom:
		// Return ebx to its previous state
		sub ebx, edx

		// Skip if there is no unvisited neighbors(prevent zero division error)
		cmp ecx, 0
		je no_neighbors

		// Save the current cell to push it to the stack later
		mov edi, ebx

		// Select a random cell
		push ecx
		call rand
		pop ecx
		xor edx, edx
		div ecx // Index of a chosen cell(in a neighbors stack) is at edx

		// Get a choosen cell and empty a neighbors stack
		empty_stack:
			dec ecx
			pop eax

			// If cell is the chosen
			cmp ecx, edx
			jne sub_condition
			mov esi, maze
			// Determine index of the chosen cell
			cmp eax, LEFT_CELL
			je left_c 
			cmp eax, RIGHT_CELL
			je right_c 
			cmp eax, TOP_CELL
			je top_c 
			// Default case: bottom cell
				// Remove a wall between cells
				add esi, ebx
				or [esi], BOTTOM_PASSAGE
				add ebx, width
				jmp sub_condition

			left_c:
				dec ebx
				// Remove a wall between cells
				add esi, ebx
				or [esi], RIGHT_PASSAGE
				jmp sub_condition
			right_c:
				// Remove a wall between cells
				add esi, ebx
				or [esi], RIGHT_PASSAGE
				inc ebx
				jmp sub_condition
			top_c:
				sub ebx, width
				// Remove a wall between cells
				add esi, ebx
				or [esi], BOTTOM_PASSAGE
				jmp sub_condition

			sub_condition:

			// While neighbors stack is not empty
			cmp ecx, 0
			jg empty_stack

		// Get stack size back
		pop ecx
		// Push the current cell to the stack
		push edi
		// Return edi to its previous state
		mov edi, visited
		// Mark the chosen cell as visited
		mov esi, edi
		add esi, ebx
		mov [esi], VISITED
		// Push it to the stack
		push ebx
		// Update stack size
		add ecx, 2
		jmp main_condition

		no_neighbors:
		// Get stack size back
		pop ecx

		main_condition:
		// While stack is not empty
		cmp ecx, 0
		jg l
	}

	// Deallocate memory
	free(visited);
}
