#ifndef rendering
#define rendering

int renderGrid(SDL_Renderer *renderer, int grid[6][8], int squareWidth, int windowWidth, int windowHeight);
int initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int screenWidth, int screenHeight);
void renderPieceOnMouse(SDL_Renderer *renderer, Piece piece, int squareWidth);
void renderPieceSelection(SDL_Renderer *renderer, Piece pieces[5], int squareWidth, int windowWidth, int windowHeight, Button *buttons, int playerCount);
//void renderPiece(SDL_Renderer *renderer, Piece piece, int squareWidth, int x, int y);

#endif
