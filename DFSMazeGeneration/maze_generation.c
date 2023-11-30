#include "maze_generation.h"

void generate_maze_binary_tree(uint8_t* const maze, const uint32_t width, const uint32_t height)
{
	__asm {
		mov edx, width
		// Counter(width - 1 to 0)
		mov ecx, edx
		sub ecx, 1
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
			xor edx, edx
			mov ebx, 2
			div ebx
			// Edx is either RIGHT_PASSAGE or BOTTOM_PASSAGE
			add edx, 1
			mov [esi], edx
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
		sub edi, 1
		l2:
			mov [esi], RIGHT_PASSAGE
			inc esi
			cmp esi, edi
			jl l2
	}
}

void generate_maze_sidewinder(uint8_t* const maze, const uint32_t width, const uint32_t height)
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
		l1:
			mov [esi], RIGHT_PASSAGE
			inc esi
			loop l1

		// Skip the last cell in the first row
		inc esi

		// Number of cells in the current run
		xor ebx, ebx
		// Counter(width - 1 to 0)
		mov ecx, width
		
		l2:
			cmp ecx, 0
			je new_row

			push esi
			inc ebx
			jmp clear_run

		new_row:
			mov ecx, width
		clear_run:
			cmp ebx, 0
			je condition
			// Save the counter and the number of cells 
			push ecx
			push ebx
			// Choose a random cell for which top passage will be carved
			call rand
			// Get values back from the stack
			pop ebx
			pop ecx
			// Random cell is at edx:
			xor edx, edx
			div ebx
			// A loop from ebx-1 to 0
			pop_cells:
				dec ebx
				// Pop a cell
				pop eax
				// If it's the selected cell
				cmp ebx, edx
				jne pop_cells_condition
				// Carve a bottom passage for the top cell
				sub eax, width
				or [eax], BOTTOM_PASSAGE

			pop_cells_condition:
				cmp ebx, 0
				jg pop_cells

		condition:
			dec ecx
			inc esi
			cmp esi, edi
			jl l2
	}
}