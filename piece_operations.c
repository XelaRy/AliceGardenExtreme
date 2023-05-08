#include <stdbool.h>
#include <stdlib.h>

#include "game_data_structures.h"
#include "piece_operations.h"

// IDEA : return a piece instead of modifying the parameter
void generatePiece(Piece* piece, int Pack) {
    // Positions[Pack][Square][0 = x, 1 = y]
    // Pack[] : 0 = I, 1 = L, 2 = O, 3 = T, 4 = S
	int positions[5][4][2] = {
	{{0,0}, {0,1}, {0,2}, {0,3}},
	{{0,0}, {0,1}, {0,2}, {1,2}},
	{{0,0}, {0,1}, {0,2}, {1,1}},
	{{0,0}, {0,1}, {1,0}, {1,1}},
	{{0,0}, {0,1}, {1,1}, {1,2}}
	};

	for (int i = 0; i < 4; i++) {
		piece->squares[i].x = positions[Pack][i][0];
		piece->squares[i].y = positions[Pack][i][1];
		piece->squares[i].symbol = (rand() % 5) + 1;
	}
}

void normalizePiece(Piece* myPiece) {
	int i;
	// Normalize the piece to the left
	int test = myPiece->squares[0].x;
	for (i = 1; i < 4; i++) {
		if (myPiece->squares[i].x < test)
			test = myPiece->squares[i].x;
	}
	for (i = 0; i < 4; i++)
		myPiece->squares[i].x -= (test);


	// Normalize the piece to the top
	test = myPiece->squares[0].y;
	for (i = 1; i < 4; i++) {
		if (myPiece->squares[i].y < test)
			test = myPiece->squares[i].y;
	}
	for (i = 0; i < 4; i++)
		myPiece->squares[i].y -= (test);
}

void rotatePiece(Piece* piece) {
    int x, y;

    for (int i = 0; i < 4; i++) {
        x = piece->squares[i].x - 4;
        y = piece->squares[i].y - 4;

        piece->squares[i].x = y;
        piece->squares[i].y = -x;
    }

    normalizePiece(piece);
}

void flipPiece(Piece* piece) {
	for (int i = 0; i < 4; i++)
		piece->squares[i].x = 4 - piece->squares[i].x;

    // DEBUG FIX LATER
    normalizePiece(piece);
}

void pieceMax(Piece piece, int* max_x, int* max_y) {
	*max_x = piece.squares[0].x;
	*max_y = piece.squares[0].y;

	for (int i = 1; i < 4; i++) {
		if (piece.squares[i].x > *max_x)
			*max_x = piece.squares[i].x;
		if (piece.squares[i].y > *max_y)
			*max_y = piece.squares[i].y;
	}
}

void pieceMin(Piece piece, int* min_x, int* min_y) {
	*min_x = piece.squares[0].x;
	*min_y = piece.squares[0].y;

	for (int i = 1; i < 4; i++) {
		if (piece.squares[i].x < *min_x)
			*min_x = piece.squares[i].x;
		if (piece.squares[i].y < *min_y)
			*min_y = piece.squares[i].y;
	}
}

void pieceMinMax(Piece piece, int* min_x, int* min_y, int* max_x, int* max_y) {
	pieceMin(piece, min_x, min_y);
	pieceMax(piece, max_x, max_y);
}

bool pieceOverlap(Piece piece, int grid[6][8], int x, int y) {
	int min_x, min_y, max_x, max_y;
	pieceMinMax(piece, &min_x, &min_y, &max_x, &max_y);

	for (int i = 0; i < 4; i++) {
		if (grid[piece.squares[i].y + y][piece.squares[i].x + x] != 0)
			return true;
	}

	return false;
}

void placePiece(Piece piece, int grid[6][8], int x, int y) {
	for (int i = 0; i < 4; i++)
		grid[piece.squares[i].y + y][piece.squares[i].x + x] = piece.squares[i].symbol;
}
