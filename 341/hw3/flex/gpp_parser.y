%{
#include <stdio.h>
#include <stdlib.h>    
#include <string.h>

#define OUT_FILE "out.txt"
#define INITIAL_SIZE 25

extern FILE *yyin;      //reads from the file
extern FILE *yyout;     //writes to the file
extern char *yytext;    //current token

extern int yyparse();
extern int yylex();
extern void yy_scan_string(const char *);
extern void yylex_destroy();

int yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", yytext);
    return 1;
}

%}

%token  IDENTIFIER VALUEF COMMENT STRING NEWLINE UNKNOWN
    
%token  KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL KW_LIST KW_APPEND KW_CONCAT
        KW_SET KW_DEFFUN KW_DEFVAR KW_FOR KW_WHILE KW_IF KW_LOAD KW_DISP KW_TRUE
        KW_FALSE KW_EXIT

%token  OP_LESS OP_GREAT OP_EQUAL OP_PLUS OP_MINUS OP_DIV OP_MULT OP_OP OP_CP
        OP_COMMA

%%
START:  INPUT;

INPUT:  EXPLIST
        | EXPI;

EXP:    OP_OP KW_DISP STRING OP_CP
        | OP_OP KW_LOAD STRING OP_CP                //(load “helloworld.g++”)
        | OP_OP KW_EXIT OP_CP   { return 0; }       //(exit)
        | OP_OP OP_PLUS EXP EXP OP_CP
        | OP_OP OP_MINUS EXP EXP OP_CP
        | OP_OP OP_MULT EXP EXP OP_CP
        | OP_OP OP_DIV EXP EXP OP_CP
        | FCALL
        | IDENTIFIER
        | VALUEF
        | STRING
        | COMMENT;

EXPI:   OP_OP KW_DEFFUN IDENTIFIER LIST EXPLIST OP_CP
        | OP_OP KW_IF EXPB EXPLIST OP_CP
        | OP_OP KW_WHILE EXPB EXPLIST OP_CP
        | OP_OP KW_FOR EXPB EXPLIST OP_CP
        | OP_OP KW_DEFVAR IDENTIFIER EXPLIST OP_CP
        | OP_OP KW_SET IDENTIFIER EXPLIST OP_CP;

EXPB:   OP_OP KW_EQUAL EXP EXP OP_CP
        | OP_OP KW_LESS VALUEF VALUEF OP_CP
        | OP_OP OP_LESS VALUEF VALUEF OP_CP
        | OP_OP OP_GREAT VALUEF VALUEF OP_CP
        | OP_OP OP_EQUAL EXP EXP OP_CP
        | OP_OP KW_AND EXPB EXPB OP_CP
        | OP_OP KW_OR EXPB EXPB OP_CP
        | OP_OP KW_NOT EXPB OP_CP
        | OP_OP KW_TRUE OP_CP
        | OP_OP KW_FALSE OP_CP;

EXPLIST: EXPLIST EXP
        | EXP;

FCALL:  OP_OP IDENTIFIER LIST OP_CP;

LIST:   OP_OP VARS OP_CP;

VARS:   VARS VALUEF
        | VARS IDENTIFIER
        | VALUEF
        | IDENTIFIER
        | KW_NIL;
%%

void display_token(int id) {
    switch (id) {
        case KW_AND:    printf("KW_AND "); break;
        case KW_OR:     printf("KW_OR "); break;
        case KW_NOT:    printf("KW_NOT "); break;
        case KW_EQUAL:  printf("KW_EQUAL "); break;
        case KW_LESS:   printf("KW_LESS "); break;
        case KW_NIL:    printf("KW_NIL "); break;
        case KW_LIST:   printf("KW_LIST "); break;
        case KW_APPEND: printf("KW_APPEND "); break;
        case KW_CONCAT: printf("KW_CONCAT "); break;
        case KW_SET:    printf("KW_SET "); break;
        case KW_DEFFUN: printf("KW_DEFFUN "); break;
        case KW_DEFVAR: printf("KW_DEFVAR "); break;
        case KW_FOR:    printf("KW_FOR "); break;
        case KW_WHILE:  printf("KW_WHILE "); break;
        case KW_IF:     printf("KW_IF "); break;
        case KW_LOAD:   printf("KW_LOAD "); break;
        case KW_DISP:   printf("KW_DISP "); break;
        case KW_TRUE:   printf("KW_TRUE "); break;
        case KW_FALSE:  printf("KW_FALSE "); break;
        case KW_EXIT:   printf("KW_EXIT "); break;

        case OP_PLUS:   printf("OP_PLUS "); break;
        case OP_MINUS:  printf("OP_MINUS "); break;
        case OP_DIV:    printf("OP_DIV "); break;
        case OP_MULT:   printf("OP_MULT "); break;
        case OP_OP:     printf("OP_OP "); break;
        case OP_CP:     printf("OP_CP "); break;
        case OP_COMMA:  printf("OP_COMMA "); break;

        case IDENTIFIER: printf("IDENTIFIER "); break;
        case VALUEF:     printf("VALUEF "); break;
        case COMMENT:    printf("COMMENT "); break;
        case STRING:     printf("STRING "); break;

        default: printf("UNKNOWN: %s\n", yytext);
    }
}

int main(int argc, char* argv[]) {
    FILE *strin = NULL;
    FILE *strout = NULL;

    int token, counter = 0, size = INITIAL_SIZE;
	int *ptr = (int *)malloc(size * sizeof(int));

	if (ptr == NULL) {
		printf("Memory not allocated!\n");
		return 1;
	}

    if (argc == 1) {
        strin = stdin;
    } else {
        strin = fopen(argv[1], "r");
        if (!strin) {
            printf("Input file %s does not exist!", argv[1]);
			free(ptr);
            return 1;
        }
    }

    strout = fopen("output.txt", "w");

    yyin = strin;
    yyout = strout;
    
    printf("\nStarting G++ Interpreter... (Type '(exit)' to quit)\n\n");

    while ((token = yylex()) != 0) {
		if (token == UNKNOWN) {
			printf("ERROR: Terminating...\n");
			return 1;
		} else if (token == NEWLINE) {
			printf("OK: ");
			for (int i = 0; i < counter; i++)
				display_token(ptr[i]);
			printf("\n");
			counter = 0;
        } else {
			if (counter >= size) {
				size *= 2;
				ptr = (int *)realloc(ptr, size * sizeof(int));

				if (ptr == NULL) {
					printf("Memory reallocation failed!\n");
					return 1;
				}
			}

			ptr[counter] = token;
			++counter;

			if (token == KW_EXIT) {
				free(ptr);
				fclose(strin);
				fclose(strout);
				return 0;
			}
		}
    }
	
	free(ptr);
    fclose(strout);

    return 0;
}