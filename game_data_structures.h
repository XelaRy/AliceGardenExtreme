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
} Square;


typedef struct {
	Square squares[4];
    bool taken;
    bool pickable;
} Piece;


typedef struct {
	char name[20];
	int board[6][8];
	int score;
	Piece piece;
} Player;


typedef struct {
    int x;
    int y;
    int w;
    int h;
    int id;
} Button;

void initButtons(Button *buttons, int squareWidth, int squareHeight, int windowWidth, int windowHeight, GameState gameState, int playerCount);

#endif
