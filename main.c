#include <stdio.h>
#include<math.h>
typedef struct square {
    int x;
    int y;
    char symbol;
} Square;


typedef struct piece {
    Square squares[4];
} Piece;


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
        char symbol = piece.squares[i].symbol;
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
    Piece myPiece = {
        {
            { 0, 0, 'X' },
            { 0, 1, 'X' },
            { 0, 2, 'X' },
            { 1, 2, 'X' }
        }
    };

    printf("Before rotation:\n");
    printPiece(myPiece);

    int rot=0,i;
    while (rot<3)
    {

    printf("\nAfter rotation:\n");
    rotatePiece(&myPiece);
//............................................................
// PLACER LA PIECE SUR LA GAUCHE
    int test=myPiece.squares[0].x;
    for(i=1;i<4;i++)
    {
        if (myPiece.squares[i].x<test)
            test=myPiece.squares[i].x;
    }
    for(i=0;i<4;i++)
    myPiece.squares[i].x-=(test);
// PLACER LA PIECE SUR LE HAUT
    test=myPiece.squares[0].y;
    for(i=1;i<4;i++)
    {
        if (myPiece.squares[i].y<test)
            test=myPiece.squares[i].y;
    }
    for(i=0;i<4;i++)
    myPiece.squares[i].y-=(test);
//............................................................
    printPiece(myPiece);
    rot+=1;
    }



    printf("\nAfter invertion:\n");
    invertPiece(&myPiece);

 //............................................................
// PLACER LA PIECE SUR LA GAUCHE
    int test=myPiece.squares[0].x;
    for(i=1;i<4;i++)
    {
        if (myPiece.squares[i].x<test)
            test=myPiece.squares[i].x;
    }
    for(i=0;i<4;i++)
    myPiece.squares[i].x-=(test);
// PLACER LA PIECE SUR LE HAUT
    test=myPiece.squares[0].y;
    for(i=1;i<4;i++)
    {
        if (myPiece.squares[i].y<test)
            test=myPiece.squares[i].y;
    }
    for(i=0;i<4;i++)
    myPiece.squares[i].y-=(test);
//............................................................


    printPiece(myPiece);
    return 0;
}
