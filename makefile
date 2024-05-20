all:
	gcc -Werror -g main.c log.c game.c map.c print.c character.c -o main -lncursesw
debug: 
	valgrind --leak-check=yes --log-file="debug.log" ./main 
wtf:
	make && DEBUGINFOD_URLS="https://debuginfod.archlinux.org" G_SLICE=always-malloc valgrind --leak-check=full --show-leak-kinds=all -s --log-file="debug.log" --track-origins=yes ./main 
