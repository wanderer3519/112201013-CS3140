%{
    #include <stdio.h>
    #include "calculator.hpp"
    #include "y.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?    { printf("NUMBER: %s\n", yytext); yylval.val = atof(yytext); return NUMBER; }
[A-Z]      { printf("VAR: %s\n", yytext); yylval.index = yytext[0] - 'A'; return VAR; }
[+\-*/=()] { printf("OP: %s\n", yytext); return *yytext; }
[ \t]    { /* Ignore whitespace */ }
\n { return '\n'; }
.          { printf("Unexpected character: %s\n", yytext); return 0; }

%%

int yywrap(){
    return 1;
}