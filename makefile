all:
	gcc -g main.c log.c game.c map.c print.c character.c -o main -lncursesw
