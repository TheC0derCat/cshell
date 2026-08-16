main: main.c
	gcc -o cshell main.c -lpthread -Werror -Wfatal-errors -Wpedantic -Wall -Wextra
	./cshell
