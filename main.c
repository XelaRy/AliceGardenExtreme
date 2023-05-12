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
#include "piece_operations.h"
#include "rendering.h"


void renderMenu(SDL_Renderer* renderer) {
}

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

void renderTextBox(SDL_Renderer* renderer, int windowWidth, int windowHeight, int x, int y, char* text, TTF_Font* font, int fontSize) {
    // Display text box
    SDL_Rect textbox_rect = { x, y, 200, 20 };
    textbox_rect.w = 10 + strlen(text) * 14;
    textbox_rect.h = fontSize + 4;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &textbox_rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &textbox_rect);
    

    // Display text
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect text_rect = { x + 6, y - 4, 0, 0 };

    SDL_QueryTexture(textTexture, NULL, NULL, &text_rect.w, &text_rect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &text_rect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// bool isPlaceable(int grid[6][8], Piece piece) {
//     // First count the empty grid squares
//     int emptySquares = 0;
//     for (int i = 0; i < 6; i++)
//         for (int j = 0; j < 8; j++)
//             if (grid[i][j] == 0)
//                 emptySquares++;
    
//     // Then checks if there are at least 4 empty squares to place the piece
//     if (emptySquares < 4)
//         return false;

//     // Then check every position and rotation combinaison of the piece to see if it can be placed
    

// }

int countAdjacentSquares(int grid[6][8], int sym, int x, int y) {
    int found = 1;
    int right = 0;
    int down = 0;

    if (x >= 8 || y >= 6) {
        return found;
    }
    if (grid[y][x + 1] == sym) {
        right = 1;
        grid[y][x + 1] = -1;
    }
    if (grid[y + 1][x] == sym) {
        down = 1;
        grid[y + 1][x] = -1;
    }

    return found + (right ? countAdjacentSquares(grid, sym, x + 1, y) : 0) + (down ? countAdjacentSquares(grid, sym, x, y + 1) : 0);
}

int maxAdjacentSymbols(int grid[6][8], int symbol) {
    int gridCopy[6][8];
    int max = 0;
    int tmp;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 7; j++)
            gridCopy[i][j] = grid[i][j];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 8; j++) {
            tmp = countAdjacentSquares(gridCopy, symbol, i, j);
            if (tmp > max)
                max = tmp;
        }

    return max;
}


bool gameEnd(int grid[6][8]) {
    // If there is less than 4 empty spaces adjacent, the player cannot place any normal piece
    return maxAdjacentSymbols(grid, 0) < 4;
}

void endScreen(Player player[],int playerCount,SDL_Renderer* renderer,int windowWidth,int windowHeight){
    bool quit=false;
    while(!quit){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    Button quitbutton;
    quitbutton.x =  windowWidth / 2 - 28;
    quitbutton.w = windowWidth * 0.8;
    quitbutton.y = windowHeight * 0.75;
    quitbutton.h = windowHeight * 0.1; 
    int x,y;
    SDL_Event event;
    SDL_GetMouseState(&x, &y);
    TTF_Init();
    int fontSize = 24;
    TTF_Font* font = TTF_OpenFont("fonts/RobotoMono-Regular.ttf", fontSize);
    SDL_StartTextInput();
    if (event.button.button == SDL_BUTTON_LEFT) { // handle left mouse button click
    int x = event.button.x;
    int y = event.button.y;
    if(x>=quitbutton.x && x<=quitbutton.x+quitbutton.w && y>=quitbutton.y && y<=quitbutton.y + quitbutton.h){
        quit=true;
    }
    } 
    renderTextBox(renderer, windowWidth, windowHeight, quitbutton.x, quitbutton.y, "QUIT", font, fontSize);
    SDL_RenderPresent(renderer);        
    SDL_Delay(10);
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
    int elapsedTime = 0;
    int bagWidth = 50;
    int squareWidth = 100;
    int gridOriginX, gridOriginY;
    Piece pieces[5];
    for (int i = 0; i < 5; i++) {
        pieces[i].taken = false;
        pieces[i].pickable = true;
    }

    // Initialize Player Variables
    int playerCount = 0;
    Player players[4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 6; j++)
            for (int k = 0; k < 8; k++)
                players[i].board[j][k] = 0;
    

    // Game Loop Exit Variable
    SDL_Event event;
    bool quit = false;
    bool initPhase = true;
    bool firstTurn = true;
    int turn = 0;
    int leader = 0;

    // Initialize Menu Variables
    char name[50] = "";
    int name_length = 0;
    bool menu = true;
    // Create a button that is 80% at the bottom of the screen centered horizontally
    Button playButton = { windowWidth / 2 - 28, windowHeight * 0.75, windowWidth * 0.8, windowHeight * 0.1, 0 };

    // Initialize TTF
    TTF_Init();
    int fontSize = 24;
    TTF_Font* font = TTF_OpenFont("fonts/RobotoMono-Regular.ttf", fontSize);
    SDL_StartTextInput();

    // Main Menu
    while (menu) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
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
                        quit = true;
                        menu = false;
                        break;
                    }
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (name[0] != '\0') {
                            strcpy(players[playerCount].name, name);
                            name[0] = '\0';
                            playerCount++;
                            if (playerCount == 4) {
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

                                if (x > playButton.x && x < playButton.x + playButton.w && y > playButton.y && y < playButton.y + playButton.h) {
                                    if (playerCount > 0) {
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

        renderTextBox(renderer, windowWidth, windowHeight, windowWidth / 2 - strlen(name) * 7, (windowHeight * 0.2 - 10) + playerCount * (windowHeight * 0.05), name, font, fontSize);

        for (int i = 0; i < playerCount; i++) {
            renderTextBox(renderer, windowWidth, windowHeight, (windowWidth / 2) - (strlen(players[i].name) * 7), (windowHeight * 0.2 - 10) + i * (windowHeight * 0.05), players[i].name, font, fontSize);
        }

        // Play Button
        renderTextBox(renderer, windowWidth, windowHeight, playButton.x, playButton.y, "Play", font, fontSize);
        
        SDL_RenderPresent(renderer);        

        SDL_Delay(10);
    }

    Button buttons[10];
    Button quitbutton;
    quitbutton.x =  windowWidth / 2 - 28;
    quitbutton.w = windowWidth * 0.8;
    quitbutton.y = windowHeight * 0.75;
    quitbutton.h = windowHeight * 0.1;  

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
                                    if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w && y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
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
                                    if(x>=quitbutton.x && x<=quitbutton.x+quitbutton.w && y>=quitbutton.y && y<=quitbutton.y + quitbutton.h){
                                        printf("HAHAHHA");
                                        endScreen(players,playerCount,renderer,windowWidth,windowHeight);


















                                        quit = true;
                                    }
                                    if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w && y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
                                        if (pieces[i].pickable) {
                                            players[turn].piece = pieces[i];
                                            pieces[i].taken = true;
                                            pieces[i].pickable = false;
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
                                    if(x>=quitbutton.x && x<=quitbutton.x+quitbutton.w && y>=quitbutton.y && y<=quitbutton.y + quitbutton.h){
                                        printf("HAHAHHA");
                                        //quit = true;
                                    }
                                    gridOriginX = (windowWidth - 8 * squareWidth) / 2;
                                    for (int j = 0; j < 8; j++) {
                                        if (x >= gridOriginX && x <= gridOriginX + squareWidth && y >= gridOriginY && y <= gridOriginY + squareWidth) {
                                            // Check if piece is hanging off the edge of the board
                                            int maxX, maxY;
                                            pieceMax(players[turn].piece, &maxX, &maxY);
                                            if (j + maxX > 7 || i + maxY > 5) {
                                                printf("Invalid Hanging\n");
                                                break;
                                            }
                                            // Check if piece is overlapping another piece
                                            if (pieceOverlap(players[turn].piece, players[turn].board, j, i)) {
                                                printf("Invalid Overlap\n");
                                                break;
                                            }
                                            // Place piece
                                            placePiece(players[turn].piece, players[turn].board, j, i);

                                            SDL_ShowCursor(SDL_ENABLE);
                                            initPhase = true;
                                            gameState = BagSelection;

                                            // DEBUG
                                            printf("Max rose : %d\n", maxAdjacentSymbols(players[turn].board, 1));

                                            turn = (turn + 1) % playerCount;
                                            if (turn == leader) {
                                                // New leader is the previous leader's left neighbour
                                                leader = (leader + playerCount - 1) % playerCount;
                                                turn = leader;
                                                for (int i = 0; i < playerCount + 1; i++) {
                                                    pieces[i].pickable = true;
                                                }
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

        renderGrid(renderer, players[turn].board, squareWidth, windowWidth, windowHeight);

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
                    // printf("Player %d's turn\n", leader);
                    initPhase = false;
                }
                
                SDL_GetMouseState(&x, &y);
                renderTextBox(renderer, windowWidth, windowHeight, quitbutton.x, quitbutton.y, "QUIT", font, fontSize);
                for (int i = 0; i < playerCount + 1; i++) {
                    if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w && y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
                        if (pieces[i].pickable) {
                            hovering = true;
                            break;
                        }  
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
                renderTextBox(renderer, windowWidth, windowHeight, quitbutton.x, quitbutton.y, "QUIT", font, fontSize);
                if (elapsedTime > 3000) {
                    // Display 'Cannot Place Piece ? / End Game' Button
                }
                else {
                    elapsedTime += 10;
                }

                renderPieceOnMouse(renderer, players[turn].piece, squareWidth);
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
