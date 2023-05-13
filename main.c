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

void scoreCount(Player player1,int* player1score){

    Player player;
    for(int i=0;i<6;i++){
        for(int j=0;j<8;j++){
            player.board[i][j]=player1.board[i][j];
        }
    }




    player.score=0;
    int i,j;
//1=ROSE, 2=Tree, 3=Gardener(play card), 4=Mushroom, 5=Chess piece
//Count for Chess pieces
for(i=0;i<6;i++){
    for(j=3;j<5;j++){
        if (player.board[i][j]==5){
            player.score+=5;
        }    }
}
//Count for Mushrooms
int mushroomCount;
for(j=0;j<8;j++){
    mushroomCount=0;
    for(i=0;i<6;i++){
        if (player.board[i][j]==4){
            mushroomCount+=1;
        }    
    }
    if (mushroomCount>1){
        player.score+=8;
    }
}
//Count for Trees
int firstTree,lastTree;
for(i=0;i<6;i++){
    firstTree=0;
    lastTree=0;
    for(j=0;j<8;j++){
        if(player.board[i][j]==2){
            if(firstTree==0){
                firstTree=j+1;
            }
            else{
                lastTree=j+2;
            }
        }
    }
    if(lastTree!=0){
        player.score+=(lastTree-firstTree);
    }
}
//Count for Roses
int AdjRoses;
for(i=0;i<6;i++){
    for(j=0;j<8;j++){
        if(player.board[i][j]==1){
            AdjRoses=countAdjacentSquares(player.board,1,i,j);
            if (AdjRoses>4){
                AdjRoses=4;
            }
            player.score+=((AdjRoses+1)*(AdjRoses+1));
            player.board[i][j]=-1;
            AdjRoses=0;
        }
    }
}
//Count for Empty squares
int Adjempty;
for(i=0;i<6;i++){
    for(j=0;j<8;j++){
        if(player.board[i][j]==0){
            Adjempty=countAdjacentSquares(player.board,0,i,j);//disable near empty squares
            player.score-=5;
            player.board[i][j]=-1;
            Adjempty=0;
        }
    }
}
*player1score=player.score;
}


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



// void endScreen(int windowWidth,int windowHeight,int name_length,char name[50],int playerCount,Player players[],SDL_Renderer* renderer){
//     bool quit = true; 
//     SDL_Event event;
//     Button quitButton = { windowWidth / 2 - 28, windowHeight * 0.75, windowWidth * 0.8, windowHeight * 0.1, 0 };

//     TTF_Init();
//     int fontSize = 44;
//     TTF_Font* font = TTF_OpenFont("fonts/RobotoMono-Regular.ttf", fontSize);
//     SDL_StartTextInput();
//     // end screen
//     while (quit) {
//         SDL_SetRenderDrawColor(renderer, 156, 255, 246, 255);
//         SDL_RenderClear(renderer);
//         while (SDL_PollEvent(&event)) {
//             switch (event.type) {

//                 case SDL_QUIT:
//                     quit = false;
//                     break;

//                 case SDL_MOUSEBUTTONDOWN:
//                             if (event.button.button == SDL_BUTTON_LEFT) {
//                                 // handle left mouse button click
//                                 int x = event.button.x;
//                                 int y = event.button.y;
//                                     if(x>=quitButton.x && x<=quitButton.x+quitButton.w && y>=quitButton.y && y<=quitButton.y + quitButton.h){
//                                         quit = false;
//                                     }
//                             }
//                     break;

//                 case SDL_WINDOWEVENT:
//                         if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
//                             windowWidth = event.window.data1;
//                             windowHeight = event.window.data2;
//                             quitButton.x =  windowWidth / 2 - 28;
//                             quitButton.y = windowHeight * 0.75;
//                             quitButton.w = windowWidth * 0.9;
//                             quitButton.h = windowHeight * 0.1;
//                         }
//                     break;  
//             }
//         }
//     //Score text
//     for (int i=0;i<playerCount;i++){
//         int score =players[i].score;
//         char charScore[4];
//         sprintf(charScore, "%d", score);
//         font = TTF_OpenFont("fonts/RobotoMono-Regular.ttf", fontSize-20);
//         char text[100] ="The score of ";
//         strcat(text,players[i].name);
//         strcat(text," is ");
//         strcat(text,charScore);
//         renderTextBox(renderer, windowWidth, windowHeight, quitButton.x-200, quitButton.y-(400+(50*i)), text, font, fontSize-20);
//     }
//     // Quit Button
//     font = TTF_OpenFont("fonts/RobotoMono-Regular.ttf", fontSize);
//     renderTextBox(renderer, windowWidth, windowHeight, quitButton.x+100, quitButton.y,"QUIT GAME        " , font, fontSize+1);
//     SDL_RenderPresent(renderer);        
//     SDL_Delay(10);
//     }
// }

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
    int elapsedTime = 0;
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
    Button quitbutton;
    quitbutton.x =  windowWidth / 2 - 58;
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
                                    // if(x>=quitbutton.x && x<=quitbutton.x+quitbutton.w && y>=quitbutton.y && y<=quitbutton.y + quitbutton.h){
                                    //     quit = true;
                                    //     endScreen(windowWidth,windowHeight,name_length,name,playerCount,players,renderer);
                                    // }
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
                                    // if(x>=quitbutton.x && x<=quitbutton.x+quitbutton.w && y>=quitbutton.y && y<=quitbutton.y + quitbutton.h){
                                    //     quit = true;
                                    //     endScreen(windowWidth,windowHeight,name_length,name,playerCount,players,renderer);
                                    // }
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
                            scoreCount(players[0],&players[0].score);
                             printf("Score  : %d\n",players[0].score);

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

        renderGrid(renderer, players[turn].board, squareWidth, windowWidth, windowHeight, spriteSheet, variations);

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
                // renderTextBox(renderer, windowWidth, windowHeight, quitbutton.x, quitbutton.y, "Endgame", font, fontSize);
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
                // renderTextBox(renderer, windowWidth, windowHeight, quitbutton.x, quitbutton.y, "Endgame", font, fontSize);
                if (elapsedTime > 3000) {
                    // Display 'Cannot Place Piece ? / End Game' Button
                }
                else {
                    elapsedTime += 10;
                }

                renderPieceOnMouse(renderer, players[turn].piece, squareWidth, spriteSheet);
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