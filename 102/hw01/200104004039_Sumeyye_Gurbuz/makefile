all: clean compile run

compile: main.c utils.c utils.h
	@echo "compiling..."
	@gcc -o test main.c utils.c

run:	
	@echo "running..."
	@./test

clean:
	@echo "cleaning..."
	@rm -f *.o
	@rm -f test
	@rm -f customeraccount.txt
