#ifndef rendering
#define rendering

void renderGrid(SDL_Renderer *renderer, int grid[6][8], int squareWidth, int windowWidth, int windowHeight, SDL_Texture *spritesheet, int variations[6][8]);
int initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int screenWidth, int screenHeight);
//void renderPieceOnMouse(SDL_Renderer *renderer, Piece piece, int squareWidth);
void renderBags(SDL_Renderer* renderer, int squareWidth, int windowWidth, int windowHeight, Button* buttons);
void renderPieceSelection(SDL_Renderer *renderer, Piece pieces[5], int squareWidth, int windowWidth, int windowHeight, Button *buttons, int playerCount, SDL_Texture *spritesheet);
void renderPieceOnMouse(SDL_Renderer *renderer, Piece piece, int squareWidth, SDL_Texture *spritesheet);
//void renderPiece(SDL_Renderer *renderer, Piece piece, int squareWidth, int x, int y);

#endif