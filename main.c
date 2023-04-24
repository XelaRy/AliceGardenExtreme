#include <stdio.h>
#include <stdlib.h>
#include<math.h>
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
}Player;



void GenPiece(int Pack,Piece *piece){

int randNum[4];
int i;
for (i=0;i<4;i++)
randNum[i]=(rand()%5) +1;

    switch(Pack){
   case 1 :{
        for (i=0;i<4;i++){
            piece->squares[i].x=1;
            piece->squares[i].y=i+1;
            piece->squares[i].symbol=randNum[i];
        break;
        }
    }


    case 2 :{
        for (i=0;i<3;i++){
            piece->squares[i].x=1;
            piece->squares[i].y=i+1;
        }
        piece->squares[4].x=2;
        piece->squares[4].y=3;
        for (i=0;i<4;i++)
        piece->squares[i].symbol=randNum[i];
        break;
    }


     case 3 :{
        for (i=0;i<3;i++){
            piece->squares[i].x=1;
            piece->squares[i].y=i+1;
        }
        piece->squares[4].x=2;
        piece->squares[4].y=2;
        for (i=0;i<4;i++)
        piece->squares[i].symbol=randNum[i];
        break;
    }


     case 4 :{
        for (i=0;i<2;i++){
            piece->squares[i].x=1;
            piece->squares[i].y=i+1;
        }
        for (i=2;i<4;i++){
            piece->squares[i].x=2;
            piece->squares[i].y=i-1;
        }
        for (i=0;i<4;i++)
        piece->squares[i].symbol=randNum[i];
        break;
    }


     case 5 :{
        for (i=0;i<2;i++){
            piece->squares[i].x=1;
            piece->squares[i].y=i+1;
        }
        for (i=2;i<4;i++){
            piece->squares[i].x=2;
            piece->squares[i].y=i;
        }
        for (i=0;i<4;i++)
        piece->squares[i].symbol=randNum[i];
        break;
    }


     case 6 :{
            piece->squares[0].x=1;
            piece->squares[0].y=1;
            piece->squares[0].symbol=randNum[1];
            break;
    }
}
}
;






void InitBoard(Board* board){
    int i,j;
    for(i=0;i<8;i++)
        {
            for(j=0;j<6;j++)
            {
                board->grid[i][j].x=i;
                board->grid[i][j].y=j;
                board->grid[i][j].symbol=0;
            }
        }
};

void TestBoard(Board* board){
int i,j;
for(i=0;i<8;i++)
    {
        for(j=0;j<6;j++)
        {
            printf("%d",board->grid[i][j].x);
            printf("%d",board->grid[i][j].y);
            printf("%d",board->grid[i][j].symbol);
            printf("   ");
        }
    printf("\n\n");
    }
};


void rotatePiece(Piece* piece) {
    int centerX = 0;
    int centerY = 0;

    // Calculate the center of the piece
    for (int i = 0; i < 4; i++) {
        centerX += piece->squares[i].x;
        centerY += piece->squares[i].y;
    }
    centerX = centerX/4;
    centerY /= 4;

    // Rotate each square around the center
    for (int i = 0; i < 4; i++) {
        int relativeX = piece->squares[i].x - (centerX);
        int relativeY = piece->squares[i].y + (centerY);
        piece->squares[i].x = centerX + relativeY;
        piece->squares[i].y = centerY - relativeX;
            piece->squares[i].x-=1;
            piece->squares[i].y+=1;
    }
    }

void invertPiece(Piece* piece){
for (int i = 0; i < 4; i++)
piece->squares[i].x=4-piece->squares[i].x;
}


void printPiece(Piece piece) {
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
        char symbol;
        if (piece.squares[i].symbol!=0)
            symbol='X';
        grid[y][x] = symbol;
    }

    // Print the grid
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
Piece Mypiece;
int choix=2;
GenPiece(choix,&Mypiece);
printPiece(Mypiece);


    return 0;
}
