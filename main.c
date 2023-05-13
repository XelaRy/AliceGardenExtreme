#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "game_data_structures.h"
#include "buttons.h"
#include "board_operations.h"
#include "piece_operations.h"
#include "rendering.h"


void renderMenu(SDL_Renderer* renderer, TTF_Font* font, int fontSize, int windowWidth, int windowHeight, int* playerCount, Player players[4], bool* quit) {
    // Initialize Menu Variables
    SDL_Event event;
    char name[50] = "";
    int name_length = 0;
    bool menu = true;
    Button playButton = { windowWidth / 2 - 28, windowHeight * 0.75, windowWidth * 0.8, windowHeight * 0.1, 0 };

    while (menu) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    *quit = true;
                    menu = false;
                    break;
                case SDL_TEXTINPUT:
                    if (name_length < 20)
                        strcat(name, event.text.text); // append new characters to name buffer
                    name_length = strlen(name);
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_BACKSPACE && name_length > 0) {
                        name[name_length - 1] = '\0'; // remove last character from name buffer
                        name_length = strlen(name);
                        break;
                    }
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        *quit = true;
                        menu = false;
                        break;
                    }
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (name[0] != '\0') {
                            strcpy(players[*playerCount].name, name);
                            name[0] = '\0';
                            (*playerCount)++;
                            if (*playerCount == 4) {
                                menu = false;
                                break;
                            }
                        }
                        break;
                    }
                case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) {
                                // handle left mouse button click
                                int x = event.button.x;
                                int y = event.button.y;

                                if (isOnButton(playButton, x, y)) {
                                    if (*playerCount > 0) {
                                        menu = false;
                                        break;
                                    }
                                }
                            }
                            break;
                case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            windowWidth = event.window.data1;
                            windowHeight = event.window.data2;
                            playButton.x =  windowWidth / 2 - 28;
                            playButton.y = windowHeight * 0.75;
                            playButton.w = windowWidth * 0.8;
                            playButton.h = windowHeight * 0.1;
                        }
                        break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderTextBox(renderer, windowWidth, windowHeight, windowWidth / 2 - strlen(name) * 7, (windowHeight * 0.2 - 10) + *playerCount * (windowHeight * 0.05), name, font, fontSize);

        for (int i = 0; i < *playerCount; i++) {
            renderTextBox(renderer, windowWidth, windowHeight, (windowWidth / 2) - (strlen(players[i].name) * 7), (windowHeight * 0.2 - 10) + i * (windowHeight * 0.05), players[i].name, font, fontSize);
        }

        // Play Button
        renderTextBox(renderer, windowWidth, windowHeight, playButton.x, playButton.y, "Play", font, fontSize);
        
        SDL_RenderPresent(renderer);        

        SDL_Delay(10);
    }
}

void renderEndScreen(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int fontSize, int windowWidth, int windowHeight, int playerCount, Player players[4], SDL_Texture* spriteSheet, int variations[6][8]) {
    // Initialize Menu Variables
    SDL_Event event;
    bool menu = true;
    int squareWidth = (windowWidth / 16) - windowWidth / 160;
    // Positions to the 4 corners of the board
    int gridPositions[4][2] = { {0, 0}, {windowWidth-8*squareWidth, 0}, {0, windowHeight-6*squareWidth}, {windowWidth-8*squareWidth, windowHeight-6*squareWidth} };
    SDL_SetWindowMinimumSize(window, 800, 800);

    while (menu) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    menu = false;
                    break;
                case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            windowWidth = event.window.data1;
                            windowHeight = event.window.data2;
                            if (windowWidth < windowHeight)
                                squareWidth = (windowWidth / 18) ;
                            else
                                squareWidth = (windowHeight / 14) ;
                            gridPositions[1][0] = windowWidth-8*squareWidth;
                            gridPositions[2][1] = windowHeight-6*squareWidth;
                            gridPositions[3][0] = windowWidth-8*squareWidth;
                            gridPositions[3][1] = windowHeight-6*squareWidth;
                        }
                        break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        menu = false;
                        break;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render Every Player's Name and Score
        for (int i = 0; playerCount > i; i++) {
            
        }

        // If there is only one player, render their grid in the center of the screen
        if (playerCount == 1) {
            renderGrid(renderer, players[0].board, windowWidth / 8, windowWidth, windowHeight, spriteSheet, variations, -1, -1, -1, -1);
            char name[40];
            char score[10];

            // Convert the score to a string
            sprintf(score, "%d", players[0].score);

            // Concatenate the name and score
            strcpy(name, players[0].name);
            strcat(name, " - ");
            strcat(name, score);

            // Render the name and score
            renderTextBox(renderer, windowWidth, windowHeight, (windowWidth / 2) - (strlen(name) * 7), windowHeight * 0.9, name, font, fontSize);
        }
        // Else Render Every Player's Grid
        else {
            renderGrid(renderer, players[0].board, squareWidth, windowWidth, windowHeight, spriteSheet, variations, 0, 0, 100, 100);

            for (int i = 1; playerCount > i; i++) {
                renderGrid(renderer, players[i].board, squareWidth, windowWidth, windowHeight, spriteSheet, variations, gridPositions[i][0], gridPositions[i][1], 100, 100);
            }
            for (int i = 0; i < playerCount; i++) {
                char name[40];
                char score[10];

                // Convert the score to a string
                sprintf(score, "%d", players[i].score);

                // Concatenate the name and score
                strcpy(name, players[i].name);
                strcat(name, " - ");
                strcat(name, score);

                // Render the name and score
                renderTextBox(renderer, windowWidth, windowHeight, (i * windowWidth / playerCount), windowHeight / 2, name, font, fontSize);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }
}


int main(int argc, char** argv) {
    srand(time(NULL));

    // SDL Variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    int windowWidth = 800;
    int windowHeight = 800;

    // Initialize SDL and exit if failed to do so
    if (initializeSDL(&window, &renderer, windowWidth, windowHeight) != 0)
        return 1;

    // Create a cursor pointer
    SDL_Cursor* pointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    // Create default cursor
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

    // Load Sprite Sheet
    SDL_Surface* spriteSheetSurface = IMG_Load("assets/spritesheet.png"); // Create a surface from the sprite sheet
    SDL_Texture* spriteSheet = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface); // Create a texture from the surface
    SDL_FreeSurface(spriteSheetSurface); // Free the surface

    // Game / Rendering Variables
    int gridOriginX, gridOriginY;
    int squareWidth = windowHeight * 0.8 / 6;
    int bagWidth = 50;

    // Initialize a Variations array to store the variations of each piece sprite on the board
    int variations[6][8];

    // Initialize Piece Selection Variable
    Piece pieces[5];
    for (int i = 0; i < 5; i++)
        pieces[i].taken = false;

    // Initialize Player Variables
    int playerCount = 0;
    Player players[4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 6; j++)
            for (int k = 0; k < 8; k++)
                players[i].board[j][k] = 0;
    
    // Game Loop Variable
    GameState gameState = BagSelection;
    SDL_Event event;
    bool quit = false;
    bool firstTurn = true;
    bool initPhase = true;
    int leader = 0;
    int turn = 0;

    // Initialize TTF
    TTF_Init();
    int fontSize = 24;
    TTF_Font* font = TTF_OpenFont("fonts/RobotoMono-Regular.ttf", fontSize);
    SDL_StartTextInput();

    // Main Menu
    renderMenu(renderer, font, fontSize, windowWidth, windowHeight, &playerCount, players, &quit);

    Button buttons[10];

    // Game Loop
    while (!quit) {

        // Handle events
        while (SDL_PollEvent(&event)) {
            switch (gameState) {
                case BagSelection:
                    switch (event.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) {
                                // handle left mouse button click
                                int x = event.button.x;
                                int y = event.button.y;

                                for (int i = 0; i < 5; i++) {
                                    if (isOnButton(buttons[i], x, y)) {
                                        if (firstTurn) {
                                            for (int j = 0; j < playerCount + 1; j++)
                                                generatePiece(pieces+j, buttons[i].id);
                                            firstTurn = false;
                                        }
                                        else
                                            for (int j = 0; j < playerCount + 1; j++) {
                                                if (pieces[j].taken)
                                                    generatePiece(pieces+j, buttons[i].id);
                                                pieces[j].taken = false;
                                            }

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
                                SDL_SetWindowMinimumSize(window, 8 * squareWidth, 800);
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
                                    if (isOnButton(buttons[i], x, y)) {
                                        if (!pieces[i].taken) {
                                            players[turn].piece = pieces[i];
                                            pieces[i].taken = true;
                                            SDL_ShowCursor(SDL_DISABLE);
                                            gameState = PiecePlacement;
                                        }
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
                                SDL_SetWindowMinimumSize(window, 8 * squareWidth, 800);
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
                            if (event.button.button == SDL_BUTTON_LEFT) {
                                int x = event.button.x;
                                int y = event.button.y;
                                // Change Later
                                gridOriginY = 0;
                                
                                // Loop over every square in the grid and check if the mouse click was inside it
                                for (int i = 0; i < 6; i++) {
                                    gridOriginX = (windowWidth - 8 * squareWidth) / 2;
                                    for (int j = 0; j < 8; j++) {
                                        if (x >= gridOriginX && x <= gridOriginX + squareWidth && y >= gridOriginY && y <= gridOriginY + squareWidth) {
                                            // Try to place piece
                                            if (!placePiece(players[turn].piece, players[turn].board, variations, j, i))
                                                break;

                                            // Show Cursor and change game state
                                            SDL_ShowCursor(SDL_ENABLE);
                                            initPhase = true;
                                            gameState = BagSelection;

                                            // Change turn
                                            turn = (turn + 1) % playerCount;
                                            if (turn == leader) {
                                                // New leader is the previous leader's left neighbour
                                                leader = (leader + playerCount - 1) % playerCount;
                                                turn = leader;
                                                quit = gameEnd(players[turn].board);
                                            }
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
                                rotatePiece(&players[turn].piece);
                            }
                            else if (event.key.keysym.sym == SDLK_f) {
                                flipPiece(&players[turn].piece);
                            }
                            break;
                        case SDL_WINDOWEVENT:
                            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                windowWidth = event.window.data1;
                                windowHeight = event.window.data2;
                                squareWidth = windowHeight * 0.8 / 6;
                                SDL_SetWindowMinimumSize(window, 8 * squareWidth, 800);
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

        renderGrid(renderer, players[turn].board, squareWidth, windowWidth, windowHeight, spriteSheet, variations, -1, -1, -1, -1);

        int x, y;
        bool hovering = false;
        switch (gameState) {
            case BagSelection:
                if (turn != leader)
                    gameState = PieceSelection;

                // Bag Selection Phase
                if (initPhase) {
                    initButtons(buttons, bagWidth, bagWidth, windowWidth, windowHeight, gameState, 0);
                    initPhase = false;
                }

                SDL_GetMouseState(&x, &y);
                for (int i = 0; i < 5; i++) {
                    if (isOnButton(buttons[i], x, y)) {
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
                    if (isOnButton(buttons[i], x, y)) {
                        if (!pieces[i].taken) {
                            hovering = true;
                            break;
                        }  
                    }
                }
                SDL_SetCursor(hovering ? pointer : cursor);

                renderPieceSelection(renderer, pieces, 20, windowWidth, windowHeight, buttons, playerCount, spriteSheet);
                break;


            case PiecePlacement:
                // Piece Placement Phase
                if (initPhase) {
                    initButtons(buttons, bagWidth, bagWidth, windowWidth, windowHeight, gameState, 0);
                    initPhase = false;
                }

                renderPieceOnMouse(renderer, players[turn].piece, squareWidth, spriteSheet);
                break;
        }
        
        SDL_RenderPresent(renderer);

        // Wait for a few milliseconds
        SDL_Delay(10);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////                     ARNAUD                        ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // PLACER ICI
    // COMPTAGE DE SCORE
    // OUI ALLER
    // TU PEUX LE FAIRE
    // JE CROIS EN TOI
    // C'EST BIENTOT FINI
    // TU VAS Y ARRIVER
    // C'EST LA DERNIERE LIGNE DROITE
    // UN DERNIER EFFORT
    // ON Y EST PRESQUE
    // SCORE COUNT GO
    // GO GO GO
    // ICI

    // End Game Screen
    renderEndScreen(renderer, window, font, fontSize, windowWidth, windowHeight, playerCount, players, spriteSheet,variations);

    // Clean up and exit
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
