#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


typedef struct square {
	int x;
	int y;
	int symbol;
} Square;


typedef struct piece {
	Square squares[4];
} Piece;


typedef struct board {
	Square grid[8][6];
} Board;


typedef struct player {
	int PlayerNumber;
	char name[20];
	Piece ChosenPiece;
	Board board;
	int score;
} Player;


// IDEA : return a piece instead of modifying the parameter
void GeneratePiece(Piece* piece, int Pack) {
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
		piece->squares[i].x = positions[Pack - 1][i][0];
		piece->squares[i].y = positions[Pack - 1][i][1];
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


void initEmptyBoard(Board* board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            board->grid[i][j].x = i;
            board->grid[i][j].y = j;
            board->grid[i][j].symbol = 0;
        }
    }
}


void testBoard(Board* board) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			printf("%d", board->grid[i][j].x);
			printf("%d", board->grid[i][j].y);
			printf("%d", board->grid[i][j].symbol);
			printf("   ");
		}
		printf("\n\n");
	}
}


void rotatePiece(Piece* piece) {
	int centerX = 0;
	int centerY = 0;

	// Calculate the center of the piece
	for (int i = 0; i < 4; i++) {
		centerX += piece->squares[i].x;
		centerY += piece->squares[i].y;
	}
	centerX = centerX / 4;
	centerY /= 4;

	// Rotate each square around the center
	for (int i = 0; i < 4; i++) {
		int relativeX = piece->squares[i].x - (centerX);
		int relativeY = piece->squares[i].y + (centerY);
		piece->squares[i].x = centerX + relativeY;
		piece->squares[i].y = centerY - relativeX;
		piece->squares[i].x -= 1;
		piece->squares[i].y += 1;
	}
}


void invertPiece(Piece* piece) {
	for (int i = 0; i < 4; i++)
		piece->squares[i].x = 4 - piece->squares[i].x;
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

void PickPieces(int RoundNumber, int NumberOfPlayers,Piece PickedPieces[]){
    printf("\nChoose a bag between 1 and 5 : \n");
    int bag;
    scanf("%d",&bag);
    printf("\n\n");
    int i;
    if (RoundNumber>1){
        for (i=0;i<NumberOfPlayers;i++){
            GeneratePiece(&PickedPieces[i],bag);
        }
    }
    else{
        for (i=0;i<NumberOfPlayers+1;i++){
            GeneratePiece(&PickedPieces[i],bag);
        }
    }
}

int main() {
    // int Roundnumber=2,NumberOfPlayers=2;
    // Piece pickedpieces[NumberOfPlayers];
    // PickPieces(Roundnumber,NumberOfPlayers,pickedpieces);
    // int i;
    // for (i=0;i<NumberOfPlayers;i++){
    //     normalizePiece(&pickedpieces[i]);
    //     DEBUG_printPiece(pickedpieces[i]);
    //     printf("\n");
    // }

    // Symbol Colors :
    int colors[6][3] = {
        { 0, 0, 0 },        // 0 : black
        { 255, 0, 0 },      // 1 : red
        { 0, 255, 0 },      // 2 : green
        { 0, 0, 255 },      // 3 : blue
        { 255, 0, 255 }     // 4 : purple
        { 255, 255, 255 },  // 5 : white
    };


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Alice's Garden",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        return 1;
    }

    // Game Loop Exit Variable
    bool quit = false;
    SDL_Event event;

    // Game Loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    // Handle Keyboard Inputs
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                    break;
                default:
                    break;
            }
        }

        // Draw to renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
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
