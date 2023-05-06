all:
	g++ -I src/include -L src/lib -o ./export/game main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

