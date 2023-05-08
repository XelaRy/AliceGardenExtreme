#ifndef buttons
#define buttons

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int id;
} Button;


void initButtons(Button *buttons, int squareWidth, int squareHeight, int windowWidth, int windowHeight, GameState gameState, int playerCount);

#endif
