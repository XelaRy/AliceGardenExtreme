#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "buttons.h"
#include "game_data_structures.h"
#include "piece_operations.h"
#include "rendering.h"


void renderMenu(SDL_Renderer* renderer) {
}


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


int main(int argc, char** argv) {
    // SDL Variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    int windowWidth = 800;
    int windowHeight = 800;

    // Initialize SDL and exit if failed to do so
    if (initializeSDL(&window, &renderer, windowWidth, windowHeight) != 0) {
        return 1;
    }
    
    GameState gameState = BagSelection;

    // Create a cursor pointer
    SDL_Cursor* pointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    // Create default cursor
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

    srand(time(NULL));
    int grid[6][8] = {0};
    int bagWidth = 50;
    int squareWidth = 100;
    int gridOriginX, gridOriginY;
    Piece pieces[5];
    Piece playerPiece;
    bool newTurn = true;
    bool initPhase = true;

    // Game Loop Exit Variable
    bool quit = false;
    SDL_Event event;

    // Game Loop
    while (!quit) {
        // Main Menu
        // Create text boxes for player names and a + and - button to add or remove players


        // renderMenu(renderer);
        // while (SDL_PollEvent(&event)) {

        // }

        bool first_turn = true;
        int playerCount = 5;

        // Create an array of n + 1 buttons
        Button buttons[10];

        // Handle events
        while (SDL_PollEvent(&event)) {
            switch (gameState) {
                case BagSelection:
                    switch (event.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) { // handle left mouse button click
                                int x = event.button.x;
                                int y = event.button.y;

                                for (int i = 0; i < 5; i++) {
                                    if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w && y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
                                        for (int j = 0; j < 5; j++)
                                            generatePiece(pieces+j, buttons[i].id);
                                        initPhase = true;
                                        gameState = PieceSelection;
                                    }
                                }
                            }
                            break;
                        case SDL_KEYDOWN:
                            // Handle Keyboard Inputs
                            if (event.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                            }
                            break;
                        case SDL_WINDOWEVENT:
                            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                windowWidth = event.window.data1;
                                windowHeight = event.window.data2;
                                squareWidth = windowHeight * 0.8 / 6;
                                SDL_SetWindowMinimumSize(window, 8 * squareWidth, 480);
                                initPhase = true;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case PieceSelection:
                    switch (event.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) { // handle left mouse button click
                                int x = event.button.x;
                                int y = event.button.y;
                                
                                for (int i = 0; i < playerCount + 1; i++) {
                                    if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w && y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
                                        playerPiece = pieces[i];
                                        SDL_ShowCursor(SDL_DISABLE);
                                        gameState = PiecePlacement;
                                    }
                                }
                            }
                            break;
                        case SDL_KEYDOWN:
                            // Handle Keyboard Inputs
                            if (event.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                            }
                            break;
                        case SDL_WINDOWEVENT:
                            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                windowWidth = event.window.data1;
                                windowHeight = event.window.data2;
                                squareWidth = windowHeight * 0.8 / 6;
                                SDL_SetWindowMinimumSize(window, 8 * squareWidth, 480);
                                initPhase = true;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case PiecePlacement:
                    switch (event.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) { // handle left mouse button click
                                int x = event.button.x;
                                int y = event.button.y;
                                // Change Later
                                gridOriginY = 0;
                                
                                // Loop over every square in the grid and check if the mouse click was inside it
                                for (int i = 0; i < 6; i++) {
                                    gridOriginX = (windowWidth - 8 * squareWidth) / 2;
                                    for (int j = 0; j < 8; j++) {
                                        if (x >= gridOriginX && x <= gridOriginX + squareWidth && y >= gridOriginY && y <= gridOriginY + squareWidth) {
                                            // Check if piece is hanging off the edge of the board
                                            int maxX, maxY;
                                            pieceMax(playerPiece, &maxX, &maxY);
                                            if (j + maxX > 7 || i + maxY > 5) {
                                                printf("Invalid Hanging\n");
                                                break;
                                            }
                                            printf("Placing piece at (%d, %d)\n", j, i);
                                            // Check if piece is overlapping another piece
                                            if (pieceOverlap(playerPiece, grid, j, i)) {
                                                printf("Invalid Overlap\n");
                                                break;
                                            }
                                            // Place piece
                                            placePiece(playerPiece, grid, j, i);

                                            SDL_ShowCursor(SDL_ENABLE);
                                            initPhase = true;
                                            gameState = BagSelection;
                                        }
                                        gridOriginX += squareWidth;
                                    }
                                    gridOriginY += squareWidth;
                                }
                            }
                            break;
                        case SDL_KEYDOWN:
                            // Handle Keyboard Inputs
                            if (event.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                            }
                            else if (event.key.keysym.sym == SDLK_r) {
                                rotatePiece(&playerPiece);
                            }
                            else if (event.key.keysym.sym == SDLK_f) {
                                flipPiece(&playerPiece);
                            }
                            break;
                        case SDL_WINDOWEVENT:
                            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                windowWidth = event.window.data1;
                                windowHeight = event.window.data2;
                                squareWidth = windowHeight * 0.8 / 6;
                                SDL_SetWindowMinimumSize(window, 8 * squareWidth, 480);
                            }
                            break;
                        default:
                            break;
                    }
            }
        }

        // Render screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        gridOriginX = renderGrid(renderer, grid, squareWidth, windowWidth, windowHeight);

        int x, y;
        bool hovering = false;
        switch (gameState) {
            case BagSelection:
                // Bag Selection Phase
                if (initPhase) {
                    initButtons(buttons, bagWidth, bagWidth, windowWidth, windowHeight, gameState, 0);
                    initPhase = false;
                }

                SDL_GetMouseState(&x, &y);
                for (int i = 0; i < playerCount + 1; i++) {
                    if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w && y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
                        hovering = true;
                        break;
                    }
                }
                SDL_SetCursor(hovering ? pointer : cursor);    
                

                renderBags(renderer, bagWidth, windowWidth, windowHeight, buttons);
                break;
            case PieceSelection:
                // Piece Selection Phase
                if (initPhase) {
                    initButtons(buttons, bagWidth, bagWidth, windowWidth, windowHeight, gameState, playerCount);
                    initPhase = false;
                }
                
                SDL_GetMouseState(&x, &y);
                for (int i = 0; i < playerCount + 1; i++) {
                    if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w && y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
                        hovering = true;
                        break;
                    }
                }
                SDL_SetCursor(hovering ? pointer : cursor);

                renderPieceSelection(renderer, pieces, 20, windowWidth, windowHeight, buttons, playerCount);
                break;
            case PiecePlacement:
                // Piece Placement Phase
                if (initPhase) {
                    initButtons(buttons, bagWidth, bagWidth, windowWidth, windowHeight, gameState, 0);
                    initPhase = false;
                }

                renderPieceOnMouse(renderer, playerPiece, squareWidth);
                break;
        }
        
        SDL_RenderPresent(renderer);

        // Wait for a few milliseconds
        SDL_Delay(10);
    }


    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
