all:
	gcc -g main.c game.c map.c print.c character.c -o main -lncursesw
