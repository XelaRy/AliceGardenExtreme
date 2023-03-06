#include <stdio.h>

#define BOARD_SIZE 10

// Define game board characters
#define WALL '#'
#define CRATE '$'
#define TARGET '.'
#define ALICE '@'
#define SPACE ' '

char board[BOARD_SIZE][BOARD_SIZE] = {
    {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
    {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}
};

int main() {
    // Print the game board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }

    return 0;
}
