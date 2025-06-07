all: clean compile clear run

clear:
	@clear

clean:
	@rm -r -f *.a *.o *.out *.log myprogram *.txt

compile:
	@gcc -o myprogram main.c logger/logger.c file_operations/file_operations.c

run:
	@./myprogram