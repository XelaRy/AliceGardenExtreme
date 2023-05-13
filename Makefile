all:
	gcc -I src/include -L src/lib -o ./export/Alice *.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -Wall
