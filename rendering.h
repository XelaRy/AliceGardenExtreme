#ifndef rendering
#define rendering

void renderGrid(SDL_Renderer *renderer, int grid[6][8], int squareWidth, int windowWidth, int windowHeight);
int initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int screenWidth, int screenHeight);
void renderPieceOnMouse(SDL_Renderer *renderer, Piece piece, int squareWidth);
//void renderPiece(SDL_Renderer *renderer, Piece piece, int squareWidth, int x, int y);

#endif
