%{
    #include "y.tab.h"
%}

%%
"and"       { return KW_AND; }
"or"        { return KW_OR; }
"not"       { return KW_NOT; }
"equal"     { return KW_EQUAL; }
"less"      { return KW_LESS; }
"nil"       { return KW_NIL; }
"list"      { return KW_LIST; }
"append"    { return KW_APPEND; }
"concat"    { return KW_CONCAT; }
"set"       { return KW_SET; }
"deffun"    { return KW_DEFFUN; }
"defvar"    { return KW_DEFVAR; }
"for"       { return KW_FOR; }
"while"     { return KW_WHILE; }
"if"        { return KW_IF; }
"load"      { return KW_LOAD; }
"print"     { return KW_DISP; }
"true"      { return KW_TRUE; }
"false"     { return KW_FALSE; }
"exit"      { return KW_EXIT; }

"<"         { return OP_LESS; }
">"         { return OP_GREAT; }
"="         { return OP_EQUAL; }
"+"         { return OP_PLUS; }
"-"         { return OP_MINUS; }
"/"         { return OP_DIV; }
"*"         { return OP_MULT; }
"("         { return OP_OP; }
")"         { return OP_CP; }
","         { return OP_COMMA; }

";;".*      { return COMMENT; }
\"[^\"]*\"  { return STRING; }

[0-9]+(":"[1-9]+)?        { return VALUEF; }
[0-9]+":"[0]+             { printf("Syntax Error: %s --> division by zero\n", yytext); return UNKNOWN; }

[a-zA-Z_][0-9a-zA-Z_]*    { return IDENTIFIER; }
[0-9]+[0-9a-zA-Z_]+       { printf("Syntax Error: %s --> leading digit\n", yytext); return UNKNOWN; }

\n                        { return NEWLINE; }
[ \t]+                    { /* ignore whitespaces, tabs, and newlines */ }

.                         { printf("Syntax Error: %s cannot be tokenized\n", yytext); return UNKNOWN; }
%%

int yywrap(void) { return 0; }