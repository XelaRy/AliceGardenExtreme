#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "game_data_structures.h"
#include "piece_operations.h"
#include "rendering.h"

typedef enum {
    BagSelection,
    PieceSelection,
    PiecePlacement
} GameState;

// Type button
typedef struct {
    int x;
    int y;
    int w;
    int h;
    int id;
} Button;

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

    // int spacing = (windowWidth - squareWidth * 5) / 6;
    // SDL_Rect squareRect;

    // for (int i = 0; i < 5; i++) {
    //     squareRect.w = squareWidth;
    //     squareRect.h = squareWidth;

    //     squareRect.x = (i + 1) * spacing + squareWidth * i;
    //     squareRect.y = windowHeight * 0.85;

    //     pos[i] = squareRect.x;
        
    //     SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    //     SDL_RenderDrawRect(renderer, &squareRect);
    // }
}

// void initBagButtons(Button* buttons, int squareWidth, int windowWidth, int windowHeight) {
//     int spacing = (windowWidth - squareWidth * 5) / 6;
//     int buttonY = windowHeight - squareWidth * 1.5;

//     for (int i = 0; i < 5; i++) {
//         buttons[i].x = (i + 1) * spacing + squareWidth * i;
//         buttons[i].y = buttonY;
//         buttons[i].w = squareWidth;
//         buttons[i].h = squareWidth;
//         buttons[i].id = i;
//     }
// }

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
    }

    for (int i = 0; i < 5; i++) {
        buttons[i].x = (i + 1) * spacing + squareWidth * i;
        buttons[i].y = buttonY;
        buttons[i].w = squareWidth;
        buttons[i].h = squareHeight;
        buttons[i].id = i;
    }
}

void renderPieceSelection(SDL_Renderer* renderer, Piece pieces[5], int squareWidth, int windowWidth, int windowHeight, Button* buttons, int playerCount) {
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

            SDL_SetRenderDrawColor(renderer, colors[pieces[i].squares[j].symbol][0],colors[pieces[i].squares[j].symbol][1],colors[pieces[i].squares[j].symbol][3],255);
            SDL_RenderDrawRect(renderer, &squareRect);
        }
    }
}


void fun(int grid[6][8]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = rand() % 6;  // assign a random value between 0 and 5
        }
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
    int pos[5];
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
        int playerCount = 4;

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
                                
                                // int bagY = windowHeight * 0.85;
                                // for (int i = 0; i < 5; i++)
                                //     if (x >= pos[i] && x <= pos[i]+bagWidth && y >= bagY && y <= bagY + bagWidth) {
                                //         for (int j = 0; j < 5; j++)
                                //             generatePiece(pieces+j, i);
                                //         gameState = PieceSelection;
                                //     }
                            }
                            break;
                        case SDL_KEYDOWN:
                            // Handle Keyboard Inputs
                            if (event.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                            }
                            else {
                                fun(grid);
                            }
                            break;
                        case SDL_WINDOWEVENT:
                            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                windowWidth = event.window.data1;
                                windowHeight = event.window.data2;
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
                            else {
                            }
                            break;
                        case SDL_WINDOWEVENT:
                            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                windowWidth = event.window.data1;
                                windowHeight = event.window.data2;
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
                                int pieceY = windowHeight * 0.8;
                                for (int i = 0; i < 5; i++)
                                    if (x >= pos[i] && x <= pos[i]+bagWidth && y >= pieceY && y <= pieceY + bagWidth) {
                                        playerPiece = pieces[i];

                                        // SWITCH TO NEXT PLAYER
                                        // SWITCH GAMESTATE
                                        SDL_ShowCursor(SDL_ENABLE);
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

        renderGrid(renderer, grid, 100, windowWidth, windowHeight);

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
                renderPieceOnMouse(renderer, playerPiece, 100);
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
