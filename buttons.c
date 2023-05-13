#include <stdbool.h>

#include "game_data_structures.h"
#include "buttons.h"

void initButtons(Button* buttons, int squareWidth, int squareHeight, int windowWidth, int windowHeight, GameState gameState, int playerCount) {
    int spacing, buttonY;
    switch(gameState) {
        case BagSelection:
            spacing = (windowWidth - squareWidth * 5) / 6;
            buttonY = windowHeight - squareWidth * 1.5;
            break;
        case PieceSelection:
            spacing = (windowWidth - squareWidth * (playerCount + 1)) / (playerCount + 2);
            buttonY = windowHeight - squareHeight * 2;
            break;
        case PiecePlacement:
            break;
    }

    for (int i = 0; i < 5; i++) {
        buttons[i].x = (i + 1) * spacing + squareWidth * i;
        buttons[i].y = buttonY;
        buttons[i].w = squareWidth;
        buttons[i].h = squareHeight;
        buttons[i].id = i;
    }
}

bool isOnButton(Button button, int x, int y) {
    return x >= button.x && x <= button.x + button.w && y >= button.y && y <= button.y + button.h;
}