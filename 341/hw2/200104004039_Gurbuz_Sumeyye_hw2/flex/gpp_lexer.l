/* DEFINITIONS */
DIGIT [0-9]
IDENTIFIER [a-zA-Z][0-9a-zA-Z]*

%option noyywrap

%%
    /* KEYWORDS */

"and"     printf("KW_AND\n");
"or"      printf("KW_OR\n");
"not"     printf("KW_NOT\n");
"equal"   printf("KW_EQUAL\n");
"less"    printf("KW_LESS\n");
"nill"    printf("KW_NILL\n");
"list"    printf("KW_LIST\n");
"append"  printf("KW_APPEND\n");
"concat"  printf("KW_CONCAT\n");
"set"     printf("KW_SET\n");
"def"     printf("KW_DEF\n");
"for"     printf("KW_FOR\n");
"if"      printf("KW_IF\n");
"load"    printf("KW_LOAD\n");
"display" printf("KW_DISPLAY\n");
"true"    printf("KW_TRUE\n");
"false"   printf("KW_FALSE\n");
"exit" {
        printf("KW_EXIT\n");
        return 0;
}

    /* OPERATORS */

"+"     printf("OP_PLUS\n");
"-"     printf("OP_MINUS\n");
"/"     printf("OP_DIV\n");
"*"     printf("OP_MULT\n");
"("     printf("OP_OP\n");
")"     printf("OP_CP\n");
","     printf("OP_COMMA\n");

    /* COMMENT */

";;".*  printf("COMMENT\n");

    /* INT.S & REAL NUM.S */

{DIGIT}+"b"[0]+ {
    printf("Syntax Error: %s is not a valid real number (division by 0)\n", yytext);
    return 0;
}
{DIGIT}+("b"{DIGIT}+)? printf("VALUEF\n");

    /* IDENTIFIERS */

{IDENTIFIER} printf("IDENTIFIER\n");
{DIGIT}+{IDENTIFIER}+ {
    printf("Syntax Error: %s is not a valid identifier (leading digit)\n", yytext);
    return 0;
}

[" ""\n"]+ ; /* ignore whitespaces & new-lines */
. {
    printf("Syntax Error: %s cannot be tokenized\n", yytext);
    return 0;
}
%%

int main(int argc, char** argv) {
    yylex();
    return 0;
}