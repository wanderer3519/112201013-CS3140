/*
 *   This file is part of SIL Compiler.
 *
 *  SIL Compiler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SIL Compiler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SIL Compiler.  If not, see <http://www.gnu.org/licenses/>.
 */
%{
    #include "tree.hpp"
    #include "y.tab.h"
    int Lineno;
    int wflag;

    extern YYSTYPE yylval;
    extern void yyerror(const char* s);
    extern std::unordered_map<const char*, int> mem;
%}


digit	[0-9]
letter	[a-zA-Z]
start \/\* 
end  \*\/ 

%%
main			{ return MAIN; }		
return 			{ return RETURN; }
begin			{ return BEG; }	
end				{ return END; }	
read			{ return READ; }
write			{ return WRITE; }	
integer			{ return T_INT; }	
boolean			{ return T_BOOL; }
begindecl			{ return DECL; }
enddecl			{ return ENDDECL; }

if				{ return IF; }
then			{ return THEN; }	
else			{ return ELSE; }	
endif			{ return ENDIF; }

do				{ return DO; }
while			{ return WHILE; }
endwhile		{ return ENDWHILE; }
for 			{ return FOR; }		
"=="			{ return EQUALEQUAL; }	
"<="			{ return LESSTHANOREQUAL; }
">="			{ return GREATERTHANOREQUAL; }
"!="			{ return NOTEQUAL; }	
AND				{ return LOGICAL_AND; }	
OR				{ return LOGICAL_OR; }
NOT				{ return LOGICAL_NOT; }
true			{ return T; }	
false			{ return F; }

{digit}+		{ 
        yylval.val = atof(yytext);
        return NUM; 
    } /* no float support */

[\-\+*/=,;()><\{\}\[\]\%]     { return yytext[0]; }

{letter}({letter}|{digit})*	{ 
        // printf("VARLEX: %s\n", yytext);
        yylval.name = yytext;
        // printf("VARLEX: %s\n", yylval.name);
        return VAR; 
    }

			
["]             { return yytext[0]; }
\n              ;
[ \t]			;
.				{ yyerror("Invalid character 234"); }		
	
\/\/(.*) ; /* Regular expression for single line comment */   

{start}.*{end} ; /* Regular expression for multi line comment */  
  
%%


int yywrap(){
    /* printf("DONE PARSING"); */
    for(auto p: mem){
        const char* key = p.first;
        int val = p.second;
        printf("mem[%s] = %d\n", key, val);
    }
    return 1;
}