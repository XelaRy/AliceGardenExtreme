#ifndef buttons_h
#define buttons_h

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int id;
} Button;

void initButtons(Button *buttons, int squareWidth, int squareHeight, int windowWidth, int windowHeight, GameState gameState, int playerCount);
bool isOnButton(Button button, int x, int y);

#endif