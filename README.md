# AssemblyMazeGeneration

A simple maze generation project written in Windows inline x86 assembly, showcasing classic algorithms implemented at a low level.

This project includes three maze generation algorithms:

- Binary tree
- Sidewinder
- Recursive backtracker (Randomized DFS).

Each algorithm demonstrates different maze characteristics and traversal patterns, all implemented in inline assembly for educational purposes.

**Key Feature**: The Recursive Backtracker (DFS) uses the CPU stack (push / pop) instead of a dynamic stack structure.

### Build & Run

1. Open the solution in Visual Studio 2022
2. Set the platform to x86
3. Build the project (`Ctrl + Shift + B`)
4. Run the executable.

### Usage

`AssemblyMazeGeneration.exe [width: 3-100] [height: 3-100] [optional seed]`

#### Parameters:

- `width`: Maze width (3–100)
- `height`: Maze height (3–100)
- `seed (optional)`: Random seed for reproducible mazes.

### Example

<img width="971" height="841" alt="image" src="https://github.com/user-attachments/assets/59a25214-8d4f-408d-8d12-20968101f601" />

### Notes

The project is intended for learning and experimentation, not production use.
