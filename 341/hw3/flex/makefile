all: clean clear compile run

clean:
	@rm -f gpp_parser lex.* y.tab.* output.*

clear:
	@clear

compile:
	@flex gpp_lexer.l
	@yacc -d gpp_parser.y
	@gcc lex.yy.c y.tab.c -o gpp_parser -lfl

run:
	@./gpp_parser input.txt