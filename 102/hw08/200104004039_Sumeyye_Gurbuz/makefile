all: clean compile clear run

compile: main.c util.c util.h
	@gcc main.c util.c -o main
	
run:
	@./main

clean:
	@rm -f main *.out
	
clear:
	@clear
