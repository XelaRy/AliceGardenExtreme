#ifndef game_data_structures
#define game_data_structures

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char symbol;
} Square;


typedef struct {
	Square squares[4];
} Piece;


typedef struct {
	Square grid[6][8];
} Board;


typedef struct {
	int PlayerNumber;
	char name[20];
	Piece ChosenPiece;
	Board board;
	int score;
} Player;

#endif
