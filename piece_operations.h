#ifndef piece_operations
#define piece_operations

void generatePiece(Piece* piece, int Pack);
void normalizePiece(Piece* myPiece);
void rotatePiece(Piece* piece);
void flipPiece(Piece* piece);
void pieceMax(Piece piece, int* max_x, int* max_y);
void pieceMin(Piece piece, int* min_x, int* min_y);
void pieceMinMax(Piece piece, int* min_x, int* min_y, int* max_x, int* max_y);
bool pieceOverlap(Piece piece, int grid[6][8], int x, int y);
bool placePiece(Piece piece, int grid[6][8], int variations[6][8], int x, int y);

#endif
