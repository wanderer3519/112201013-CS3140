%{ 
	#include "y.tab.h" 
%}

%% 
[ \t]+ ; /* For skipping whitespaces */ 

[0-9]+(\.[0-9]+)? {
    /* Includes floating point numbers also */
	yylval.val = atof(yytext); 
	return NUMBER; 
} 

[A-Z]+ { 
    /* Variables */   
    yylval.index = yytext[0] - 'A';
    return VAR; 
}

\n { return '\n'; } /*  Newlines */
.	{ return yytext[0]; } /* Rest all tokens returned same as before */ 

%%
 
int yywrap(){
    return 1;
}