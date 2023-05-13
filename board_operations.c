#include <stdbool.h>

#include "board_operations.h"

// Recursive function to count the number of adjacent squares of the same symbol
// Returns the number of adjacent squares
int countAdjacentSquares(int grid[6][8], int sym, int x, int y) {
    int found = 1;
    int right = 0;
    int down = 0;

    // If the square is not the same symbol or is already counted, return 0
    if (grid[y][x] != sym && grid[y][x] != -1) {
        return 0;
    }
    // If the coordinates are on the edge of the grid, return 1
    if (x >= 8 || y >= 6) {
        return found;
    }
    // Search for adjacent squares
    // If the square is the same symbol, set it to -1 to avoid counting it again
    if (grid[y][x + 1] == sym) {
        right = 1;
        grid[y][x + 1] = -1;
    }
    if (grid[y + 1][x] == sym) {
        down = 1;
        grid[y + 1][x] = -1;
    }

    // Return the number of adjacent squares and recursively call the function for each adjacent square
    return found + (right ? countAdjacentSquares(grid, sym, x + 1, y) : 0) + (down ? countAdjacentSquares(grid, sym, x, y + 1) : 0);
}

// Returns the maximum number of adjacent squares of the same symbol
int maxAdjacentSymbols(int grid[6][8], int symbol) {
    int gridCopy[6][8];
    int max = 0;
    int tmp;

    // Copy the grid to avoid modifying the original
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 8; j++)
            gridCopy[i][j] = grid[i][j];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 8; j++) {
            // If the square is already counted, skip it
            if (gridCopy[i][j] != -1)
                tmp = countAdjacentSquares(gridCopy, symbol, i, j);
            // If the number of adjacent squares is greater than the current max, set it as the new max
            if (tmp > max)
                max = tmp;
        }

    return max;
}

// Returns true if the game has ended
// The game ends when there are less than 5 empty spaces adjacent
bool gameEnd(int grid[6][8]) {
    // If there is less than 4 empty spaces adjacent, the player cannot place any normal piece
    return maxAdjacentSymbols(grid, 0) < 5;
}
