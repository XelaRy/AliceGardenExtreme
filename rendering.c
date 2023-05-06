#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "game_data_structures.h"
#include "rendering.h"


int initializeSDL(SDL_Window** window, SDL_Renderer** renderer, int screenWidth, int screenHeight) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    *window = SDL_CreateWindow(
        "Alice's Garden",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_OPENGL
    );

    SDL_SetWindowResizable(*window, SDL_TRUE);

    if (*window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, 0);

    if (*renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}


void renderGrid(SDL_Renderer* renderer, int grid[6][8], int squareWidth, int windowWidth, int windowHeight)
{
    // Symbol Colors :
    int colors[6][3] = {
        { 0, 0, 0 },        // 0 : black
        { 255, 0, 0 },      // 1 : red
        { 0, 255, 0 },      // 2 : green
        { 0, 0, 255 },      // 3 : blue
        { 255, 0, 255 },    // 4 : purple
        { 255, 255, 255 }   // 5 : white
    };

    SDL_Rect squareRect;
    squareRect.w = squareWidth;
    squareRect.h = squareWidth;

    int offsetX = (windowWidth - squareWidth * 8) / 2;

    // Load the texture for the grass
    SDL_Surface* grass = IMG_Load("textures/erbe.jpg");
    SDL_Texture* grassTexture = SDL_CreateTextureFromSurface(renderer, grass);
    SDL_FreeSurface(grass);

    // Load the texture for the chessboard
    SDL_Surface* chessboard = IMG_Load("textures/echec.jpg");
    SDL_Texture* chessboardTexture = SDL_CreateTextureFromSurface(renderer, chessboard);
    SDL_FreeSurface(chessboard);

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 8; x++) {
            squareRect.x = offsetX + x * squareWidth;
            squareRect.y = y * squareWidth;
            if (grid[y][x] == 0) {
                // Render the texture instead of the square
                
                (x == 3 || x == 4) ?
                SDL_RenderCopy(renderer, chessboardTexture, NULL, &squareRect):
                SDL_RenderCopy(renderer, grassTexture, NULL, &squareRect);
            } else {
                // Set the color of the square based on the value in the grid
                SDL_SetRenderDrawColor(renderer, colors[grid[y][x]][0], colors[grid[y][x]][1], colors[grid[y][x]][2], 255);

                // Render the square
                SDL_RenderFillRect(renderer, &squareRect);
            }
        }
    }

    // Destroy the texture when done
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(chessboardTexture);
}


void renderPieceOnMouse(SDL_Renderer* renderer, Piece piece, int squareWidth) {
    // Symbol Colors :
    int colors[6][3] = {
        { 0, 0, 0 },        // 0 : black
        { 255, 0, 0 },      // 1 : red
        { 0, 255, 0 },      // 2 : green
        { 0, 0, 255 },      // 3 : blue
        { 255, 0, 255 },    // 4 : purple
        { 255, 255, 255 }   // 5 : white
    };
    SDL_Rect squareRect;
    int offset = squareWidth / 2;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Loop for every square
    for (int j = 0; j < 4; j++) {
        squareRect.w = squareWidth;
        squareRect.h = squareWidth;

        squareRect.x = mouseX - offset + piece.squares[j].x * squareWidth;
        squareRect.y = mouseY - offset + piece.squares[j].y * squareWidth;

        SDL_SetRenderDrawColor(renderer, colors[piece.squares[j].symbol][0],colors[piece.squares[j].symbol][1],colors[piece.squares[j].symbol][3],255);
        SDL_RenderFillRect(renderer, &squareRect);
    }
}