#ifndef game_data_structures
#define game_data_structures

typedef enum {
    BagSelection,
    PieceSelection,
    PiecePlacement
} GameState;


typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char symbol;
    unsigned char variant;
} Square;


typedef struct {
	Square squares[4];
    bool taken;
} Piece;


typedef struct {
	char name[20];
	Square board[6][8];
	int score;
	Piece piece;
} Player;

#endif