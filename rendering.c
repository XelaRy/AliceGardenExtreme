#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "game_data_structures.h"
#include "piece_operations.h"
#include "buttons.h"
#include "rendering.h"

const int colors[6][3] = {
    { 0, 0, 0 },        // 0 : black
    { 255, 0, 0 },      // 1 : red
    { 0, 255, 0 },      // 2 : green
    { 0, 0, 255 },      // 3 : blue
    { 255, 0, 255 },    // 4 : purple
    { 255, 255, 255 }   // 5 : white
};

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

// Use spritesheet texture
void renderGrid(SDL_Renderer* renderer, int grid[6][8], int squareWidth, int windowWidth, int windowHeight, SDL_Texture* spritesheet, int variations[6][8]) {
    SDL_Rect squareRect;
    squareRect.w = squareWidth;
    squareRect.h = squareWidth;

    SDL_Rect spriteRect;
    spriteRect.w = 64;
    spriteRect.h = 64;

    int offsetX = (windowWidth - squareWidth * 8) / 2;

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 8; x++) {
            squareRect.x = offsetX + x * squareWidth;
            squareRect.y = y * squareWidth;
            if (grid[y][x] == 0) {
                // Render the texture instead of the square

                // Select the right sprite,
                spriteRect.x = (x == 3 || x == 4) ? 0 : 64;
                spriteRect.y = 0;

                // Render the sprite
                SDL_RenderCopy(renderer, spritesheet, &spriteRect, &squareRect);
            } else {
                // Use the corresponding y and the variation x to select the right sprite
                spriteRect.y = grid[y][x] * 64;
                spriteRect.x = variations[y][x] * 64;

                // Render the sprite
                SDL_RenderCopy(renderer, spritesheet, &spriteRect, &squareRect);
            }
        }
    }
}

void renderPiece(SDL_Renderer* renderer, Piece piece, int squareWidth, int x, int y) {
    SDL_Rect squareRect;
    int offset = squareWidth / 2;

    // Loop for every square
    for (int j = 0; j < 4; j++) {
        squareRect.w = squareWidth;
        squareRect.h = squareWidth;

        squareRect.x = x - offset + piece.squares[j].x * squareWidth;
        squareRect.y = y - offset + piece.squares[j].y * squareWidth;

        SDL_SetRenderDrawColor(renderer, colors[piece.squares[j].symbol][0], colors[piece.squares[j].symbol][1], colors[piece.squares[j].symbol][2], 255);
        SDL_RenderFillRect(renderer, &squareRect);
    }
}

// This function uses the spritesheet to render every square of the piece with the right sprite
// First it loads the spritesheet, then it renders every square with the right sprite
void renderPieceWithSpritesheet(SDL_Renderer* renderer, Piece piece, int squareWidth, int x, int y) {
    SDL_Rect squareRect;
    int offset = squareWidth / 2;

    // Load the spritesheet
    SDL_Surface* spritesheet = IMG_Load("textures/spritesheet.png");
    SDL_Texture* spritesheetTexture = SDL_CreateTextureFromSurface(renderer, spritesheet);
    SDL_FreeSurface(spritesheet);

    // Loop for every square
    for (int j = 0; j < 4; j++) {
        squareRect.w = squareWidth;
        squareRect.h = squareWidth;

        squareRect.x = x - offset + piece.squares[j].x * squareWidth;
        squareRect.y = y - offset + piece.squares[j].y * squareWidth;

        // Set the right sprite
        SDL_Rect spriteRect;
        spriteRect.w = 64;
        spriteRect.h = 64;
        spriteRect.x = piece.squares[j].symbol * 64;
        spriteRect.y = 0;

        // Render the square
        SDL_RenderCopy(renderer, spritesheetTexture, &spriteRect, &squareRect);
    }

    // Destroy the texture when done
    SDL_DestroyTexture(spritesheetTexture);
}

void renderPieceOnMouse(SDL_Renderer* renderer, Piece piece, int squareWidth) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    renderPiece(renderer, piece, squareWidth, mouseX, mouseY);
}

// void renderPieceOnMouse(SDL_Renderer* renderer, Piece piece, int squareWidth) {
//     SDL_Rect squareRect;
//     int offset = squareWidth / 2;

//     int mouseX, mouseY;
//     SDL_GetMouseState(&mouseX, &mouseY);

//     // Loop for every square
//     for (int j = 0; j < 4; j++) {
//         squareRect.w = squareWidth;
//         squareRect.h = squareWidth;

//         squareRect.x = mouseX - offset + piece.squares[j].x * squareWidth;
//         squareRect.y = mouseY - offset + piece.squares[j].y * squareWidth;

//         SDL_SetRenderDrawColor(renderer, colors[piece.squares[j].symbol][0],colors[piece.squares[j].symbol][1],colors[piece.squares[j].symbol][2],255);
//         SDL_RenderFillRect(renderer, &squareRect);
//     }
// }

void renderBags(SDL_Renderer* renderer, int squareWidth, int windowWidth, int windowHeight, Button* buttons) {
    // Convert buttons to SDL_Rect
    SDL_Rect buttonRect;
    for (int i = 0; i < 5; i++) {
        buttonRect.x = buttons[i].x;
        buttonRect.y = buttons[i].y;
        buttonRect.w = buttons[i].w;
        buttonRect.h = buttons[i].h;
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderDrawRect(renderer, &buttonRect);
    }
}

void renderPieceSelection(SDL_Renderer* renderer, Piece pieces[5], int squareWidth, int windowWidth, int windowHeight, Button* buttons, int playerCount) {
    SDL_Rect squareRect;
    int max_X, max_Y;

    pieceMax(pieces[0], &max_X, &max_Y);
    buttons[0].w = squareWidth * (max_X + 1);
    buttons[0].h = squareWidth * (max_Y + 1);
    initButtons(buttons, buttons[0].w, buttons[0].h, windowWidth, windowHeight, PieceSelection, playerCount);
    // Loop for each piece
    for (int i = 0; i < 5; i++) {
        // Loop for every square
        for (int j = 0; j < 4; j++) {
            squareRect.w = squareWidth;
            squareRect.h = squareWidth;

            // Using the button[i] to get the x position of the square
            squareRect.x = buttons[i].x + squareWidth * pieces[i].squares[j].x;
            squareRect.y = buttons[i].y + squareWidth * pieces[i].squares[j].y;

            //squareRect.x = (i + 1) * spacing + squareWidth * i + squareWidth * pieces[i].squares[j].x;
            //squareRect.y = windowHeight * 0.8 + squareWidth * pieces[i].squares[j].y;

            if (pieces[i].taken) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            else {
            SDL_SetRenderDrawColor(renderer, colors[pieces[i].squares[j].symbol][0],colors[pieces[i].squares[j].symbol][1],colors[pieces[i].squares[j].symbol][2],255);
            }
            SDL_RenderDrawRect(renderer, &squareRect);
        }
    }
}
