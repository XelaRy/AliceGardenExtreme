#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "game_data_structures.h"


// IDEA : return a piece instead of modifying the parameter
void generatePiece(Piece* piece, int Pack) {
    // Positions[Pack][Square][0 = x, 1 = y]
    // Pack[] : 0 = I, 1 = L, 2 = O, 3 = T, 4 = S
	int positions[5][4][2] = {
	{{0,0}, {0,1}, {0,2}, {0,3}},
	{{0,0}, {0,1}, {0,2}, {1,2}},
	{{0,0}, {0,1}, {0,2}, {1,1}},
	{{0,0}, {0,1}, {1,0}, {1,1}},
	{{0,0}, {0,1}, {1,1}, {1,2}}
	};

	for (int i = 0; i < 4; i++) {
		piece->squares[i].x = positions[Pack][i][0];
		piece->squares[i].y = positions[Pack][i][1];
		piece->squares[i].symbol = (rand() % 5) + 1;
	}
}


void normalizePiece(Piece* myPiece) {
	int i;
	// Normalize the piece to the left
	int test = myPiece->squares[0].x;
	for (i = 1; i < 4; i++) {
		if (myPiece->squares[i].x < test)
			test = myPiece->squares[i].x;
	}
	for (i = 0; i < 4; i++)
		myPiece->squares[i].x -= (test);


	// Normalize the piece to the top
	test = myPiece->squares[0].y;
	for (i = 1; i < 4; i++) {
		if (myPiece->squares[i].y < test)
			test = myPiece->squares[i].y;
	}
	for (i = 0; i < 4; i++)
		myPiece->squares[i].y -= (test);
}

void rotatePiece(Piece* piece) {
    int x, y;

    for (int i = 0; i < 4; i++) {
        x = piece->squares[i].x - 4;
        y = piece->squares[i].y - 4;

        piece->squares[i].x = y;
        piece->squares[i].y = -x;
    }

    normalizePiece(piece);
}


void flipPiece(Piece* piece) {
	for (int i = 0; i < 4; i++)
		piece->squares[i].x = 4 - piece->squares[i].x;

    // DEBUG FIX LATER
    normalizePiece(piece);
}


void DEBUG_printPiece(Piece piece) {
	char grid[10][10];

	// Initialize the grid with spaces
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			grid[i][j] = ' ';
		}
	}

	// Fill in the squares of the piece
	for (int i = 0; i < 4; i++) {
		int x = piece.squares[i].x;
		int y = piece.squares[i].y;

		if (piece.squares[i].symbol != 0)
			grid[y][x] = 'X';
	}

	// Print the grid
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%c ", grid[i][j]);
		}
		printf("\n");
	}
}


void renderMenu(SDL_Renderer* renderer) {
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

void renderBags(SDL_Renderer* renderer, int squareWidth, int windowWidth, int windowHeight, int pos[5]) {
    int spacing = (windowWidth - squareWidth * 5) / 6;
    SDL_Rect squareRect;

    for (int i = 0; i < 5; i++) {
        squareRect.w = squareWidth;
        squareRect.h = squareWidth;

        squareRect.x = (i + 1) * spacing + squareWidth * i;
        squareRect.y = windowHeight * 0.85;

        pos[i] = squareRect.x;
        
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderDrawRect(renderer, &squareRect);
    }
}

void renderPieceSelection(SDL_Renderer* renderer, Piece pieces[5], int squareWidth, int windowWidth, int windowHeight, int pos[5]) {
    // Symbol Colors :
    int colors[6][3] = {
        { 0, 0, 0 },        // 0 : black
        { 255, 0, 0 },      // 1 : red
        { 0, 255, 0 },      // 2 : green
        { 0, 0, 255 },      // 3 : blue
        { 255, 0, 255 },    // 4 : purple
        { 255, 255, 255 }   // 5 : white
    };
    int spacing = (windowWidth - squareWidth * 5) / 6;
    SDL_Rect squareRect;

    // Loop for each piece
    for (int i = 0; i < 5; i++) {
        // Loop for every square
        pos[i] = (i + 1) * spacing + squareWidth * i;
        for (int j = 0; j < 4; j++) {
            squareRect.w = squareWidth;
            squareRect.h = squareWidth;

            squareRect.x = (i + 1) * spacing + squareWidth * i + squareWidth * pieces[i].squares[j].x;
            squareRect.y = windowHeight * 0.8 + squareWidth * pieces[i].squares[j].y;

            SDL_SetRenderDrawColor(renderer, colors[pieces[i].squares[j].symbol][0],colors[pieces[i].squares[j].symbol][1],colors[pieces[i].squares[j].symbol][3],255);
            SDL_RenderDrawRect(renderer, &squareRect);
        }
    }
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


void fun(int grid[6][8]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = rand() % 6;  // assign a random value between 0 and 5
        }
    }
}

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

typedef enum {
    BagSelection,
    PieceSelection,
    PiecePlacement
} GameState;

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

    srand(time(NULL));
    int grid[6][8] = {0};
    int bagWidth = 50;
    int pos[5];
    Piece pieces[5];
    Piece playerPiece;

    // Game Loop Exit Variable
    bool quit = false;
    SDL_Event event;

    // Game Loop
    while (!quit) {
        // Main Menu
        // renderMenu(renderer);
        // while (SDL_PollEvent(&event)) {

        // }


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
                                int bagY = windowHeight * 0.85;
                                for (int i = 0; i < 5; i++)
                                    if (x >= pos[i] && x <= pos[i]+bagWidth && y >= bagY && y <= bagY + bagWidth) {
                                        for (int j = 0; j < 5; j++)
                                            generatePiece(pieces+j, i);
                                        gameState = PieceSelection;
                                    }
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
                                int pieceY = windowHeight * 0.8;
                                for (int i = 0; i < 5; i++)
                                    if (x >= pos[i] && x <= pos[i]+bagWidth && y >= pieceY && y <= pieceY + bagWidth) {
                                        playerPiece = pieces[i];
                                        SDL_ShowCursor(SDL_DISABLE);
                                        gameState = PiecePlacement;
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
                                        DEBUG_printPiece(playerPiece);

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

        // Update game state

        // Render screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderGrid(renderer, grid, 100, windowWidth, windowHeight);

        switch (gameState) {
            case BagSelection:
                // Bag Selection Phase
                renderBags(renderer, bagWidth, windowWidth, windowHeight, pos);
                break;
            case PieceSelection:
                // Piece Selection Phase
                renderPieceSelection(renderer, pieces, 20, windowWidth, windowHeight, pos);
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
