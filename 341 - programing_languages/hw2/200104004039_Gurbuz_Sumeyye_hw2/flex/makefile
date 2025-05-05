all: clear compile run

clear:
	@clear

compile:
	@flex -o gpp_lexer.c gpp_lexer.l
	@gcc gpp_lexer.c

run:
	@./a.out