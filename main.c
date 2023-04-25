#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


typedef struct square {
	int x;
	int y;
	int symbol;
} Square;


typedef struct piece {
	Square squares[4];
} Piece;


typedef struct board {
	Square grid[8][6];
} Board;


typedef struct player {
	int PlayerNumber;
	char name[20];
	Piece ChosenPiece;
	Board board;
	int score;
}Player;


// IDEA : renvoyer une piece au lieu de modifier la piece en parametre
void GeneratePiece(Piece* piece, int Pack) {
    // Positions[Pack][Square][0 = x, 1 = y] 
    // Pack[] : 0 = I, 1 = L, 2 = O, 3 = T, 4 = S
	int positions[5][4][2] = {
	{{0,0}, {0,1}, {0,2}, {0,3}},	// Pack 1
	{{0,0}, {0,1}, {0,2}, {1,2}},	// Pack 2
	{{0,0}, {0,1}, {0,2}, {1,1}},	// Pack 3
	{{0,0}, {0,1}, {1,0}, {1,1}},	// Pack 4
	{{0,0}, {0,1}, {1,1}, {1,2}}	// Pack 5
	};

	for (int i = 0; i < 4; i++) {
		piece->squares[i].x = positions[Pack - 1][i][0];
		piece->squares[i].y = positions[Pack - 1][i][1];
		piece->squares[i].symbol = (rand() % 5) + 1;
	}
}


void PositionPiece(Piece* myPiece) {
	//Align piece on left and up
	//............................................................
	int i;
	// PLACER LA PIECE SUR LA GAUCHE
	int test = myPiece->squares[0].x;
	for (i = 1; i < 4; i++)
	{
		if (myPiece->squares[i].x < test)
			test = myPiece->squares[i].x;
	}
	for (i = 0; i < 4; i++)
		myPiece->squares[i].x -= (test);


	// PLACER LA PIECE SUR LE HAUT
	test = myPiece->squares[0].y;
	for (i = 1; i < 4; i++)
	{
		if (myPiece->squares[i].y < test)
			test = myPiece->squares[i].y;
	}
	for (i = 0; i < 4; i++)
		myPiece->squares[i].y -= (test);
	//............................................................
}


void InitBoard(Board* board) {
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 6; j++)
		{
			board->grid[i][j].x = i;
			board->grid[i][j].y = j;
			board->grid[i][j].symbol = 0;
		}
	}
}


void TestBoard(Board* board) {
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 6; j++)
		{
			printf("%d", board->grid[i][j].x);
			printf("%d", board->grid[i][j].y);
			printf("%d", board->grid[i][j].symbol);
			printf("   ");
		}
		printf("\n\n");
	}
}


void rotatePiece(Piece* piece) {
	int centerX = 0;
	int centerY = 0;

	// Calculate the center of the piece
	for (int i = 0; i < 4; i++) {
		centerX += piece->squares[i].x;
		centerY += piece->squares[i].y;
	}
	centerX = centerX / 4;
	centerY /= 4;

	// Rotate each square around the center
	for (int i = 0; i < 4; i++) {
		int relativeX = piece->squares[i].x - (centerX);
		int relativeY = piece->squares[i].y + (centerY);
		piece->squares[i].x = centerX + relativeY;
		piece->squares[i].y = centerY - relativeX;
		piece->squares[i].x -= 1;
		piece->squares[i].y += 1;
	}
}


void invertPiece(Piece* piece) {
	for (int i = 0; i < 4; i++)
		piece->squares[i].x = 4 - piece->squares[i].x;
}


void printPiece(Piece piece) {
	char grid[10][10];

	// Initialize the grid with spaces
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			grid[i][j] = ' ';
		}
	}

	// Fill in the squares of the piece
	for (int i = 0; i < 4; i++) {
		int x = piece.squares[i].x;
		int y = piece.squares[i].y;

		// i c'est 0 la case est vide, sinon elle a un signe, on verra après pour les diférencier
		if (piece.squares[i].symbol != 0)
			grid[y][x] = 'X';
	}

	// Print the grid
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%c ", grid[i][j]);
		}
		printf("\n");
	}
}


int main() {
	int choix;
	for (choix = 1; choix < 7; choix++) {
		Piece Mypiece;
		GeneratePiece(&Mypiece, choix);
		PositionPiece(&Mypiece);
		printPiece(Mypiece);
		printf("\n\n");
	}

	return 0;
}
