%{
    #include <stdio.h>
    #include "calculator.hpp"
    #include "y.tab.h"
    extern void yyerror(char* s);
%}

%%
[0-9]+(\.[0-9]+)?    { 
        printf("NUMBER: %s\n", yytext); 
        yylval.val = atof(yytext); 
        return NUMBER; 
    }

[A-Z]      { 
        printf("VAR: %s\n", yytext); 
        yylval.index = yytext[0] - 'A'; 
        return VAR; 
    }

[+\-*/=()] { 
        printf("OP: %s\n", yytext); 
        return *yytext; 
    }

[ \t]    ;
\n { return '\n'; }

.          { 
        yyerror("Unexpected char"); 
        return 0; 
    }

%%

int yywrap(){
    return 1;
}