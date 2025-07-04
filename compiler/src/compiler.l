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
    #include <bits/stdc++.h>
    using namespace std;
    
    

    int wflag;
    extern int lineno;

    extern YYSTYPE yylval;
    extern void yyerror(const char* s);
    extern unordered_map<string, pair<int, vector<int>> >mem;
%}


digit	[0-9]
letter	[a-zA-Z]
number [0-9]+(\.[0-9]+)?
start \/\* 
end  \*\/ 
string \"[^\"]*\"


%%
[ \t]+		    ;
[\n]           { lineno++; }
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
true			{ yylval.val = 1; return T; }	
false			{ yylval.val = 0; return F; }
break           { return BREAK; }

{digit}+		{ 
        yylval.val = atoi(yytext);
        return NUM; 
    } /* no float support */

[\-\+*/=,;()><\{\}\[\]\%]     { return yytext[0]; }

{letter}({letter}|{digit})*	{ 
        yylval.name = strdup(yytext);
        return VAR; 
    }


{string} {
        yylval.name = strndup(yytext + 1, strlen(yytext) - 2);
        return STRING;
    }



\/\/(.*) ; /* Regular expression for single line comment */   

{start}.*{end} ; /* Regular expression for multi line comment */  

.	{ 
        // char* str = "Invalid character";
        yyerror("Invalid char"); 
    
    }		
	

  
%%


int yywrap(){
    printf("\nPrinting symbol table\n");
    /* for(auto p: mem){
        // P: pair of string, pair<int, vector<int>> 

        string key = p.first;
        // int key1 = p.first;
        int val = p.second.first;

        cout << "mem[" << key << "] = ";

        if(!p.second.second.empty()){
            for(auto num: p.second.second){
                cout <<  num << ", ";        
            }
            cout << "NULL\n";
        }
        else
            cout << val << '\n';
    } */

    print_symbol_table(mem);


    return 1;
}