output: main.o parse.o cmd.o formats.o
	gcc main.o parse.o cmd.o formats.o -o output -lm



formats.o: formats.c
	gcc -c formats.c

main.o: main.c
	gcc -c main.c

parse.o: parse.c
	gcc -c parse.c

cmd.o: cmd.c
	gcc -c cmd.c



clean:
	rm *.o output