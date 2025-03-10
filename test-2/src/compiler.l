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
    
    
    int Lineno;
    int wflag;

    extern YYSTYPE yylval;
    extern void yyerror(const char* s);
    extern unordered_map<string, pair<int, vector<int>> >mem;
    extern unordered_map<string, pair<float, vector<float>> >fmem;

    extern unordered_map< string, vector< vector<int> > > mem2d;
	extern unordered_map< string, vector< vector<float> >> fmem2d;

%}


digit	[0-9]
letter	[a-zA-Z]
start \/\* 
end  \*\/ 

%%
[ \t\n]+			;
main			{ return MAIN; }		
return 			{ return RETURN; }
begin			{ return BEG; }	
end				{ return END; }	
read			{ return READ; }
write			{ return WRITE; }	
integer			{ return T_INT; }	
boolean			{ return T_BOOL; }
float           { return T_FLOAT; }
decl		{ return DECL; }
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
        yylval.val = atoi(yytext);
        return NUM; 
    } /* no float support */

{digit}+(\.{digit}+) {
    yylval.fval = atof(yytext);
    return FLOAT;
}

[\-\+*/=,;()><\{\}\[\]\%]     { return yytext[0]; }

{letter}({letter}|{digit})*	{ 
        // printf("VARLEX: %s\n", yytext);
        yylval.name = strdup(yytext);
        // printf("VARLEX: %s\n", yylval.name);
        return VAR; 
    }

			
["]             { return yytext[0]; }

\/\/(.*) ; /* Regular expression for single line comment */   

{start}.*{end} ; /* Regular expression for multi line comment */  

.	{ 
        // char* str = "Invalid character";
        yyerror("Invalid char"); 
    }		
	
%%


int yywrap(){
    printf("\nDONE PARSING...\nPrinting symbol table\n");
    cout << "INT MEMORY\n";
    for(auto p: mem){
        /* P: pair of string, pair<int, vector<int>> */

        string key = p.first;
        /* int key1 = p.first; */
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
    }

    cout << "FLOAT MEMORY" << '\n';
    for(auto p: fmem){
        /* P: pair of string, pair<int, vector<int>> */

        string key = p.first;
        /* int key1 = p.first; */
        float val = p.second.first;

        cout << "fmem[" << key << "] = ";

        if(!p.second.second.empty()){
            /* for(auto num: p.second.second){
                cout <<  num << ", ";        
            }
            cout << "NULL\n"; */
            for(int i = 0; i < p.second.second.size() - 1; i++){
                cout << p.second.second[i] << ' ';
            }
            cout << '\n';
        }
        else
            cout << val << '\n';
    }

    cout << "2d INT mem\n";
    for(auto p: mem2d){
        string arrName = p.first;
        vector<vector<int>> arr = p.second;

        cout << arrName << '\n';
        for(int r = 0; r < arr.size(); r++){
            for(int c = 0; c < arr[0].size(); c++){
                cout << arr[r][c] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }

    cout << "2d FLOAT mem\n";
    for(auto p: fmem2d){
        string arrName = p.first;
         vector<vector<float>> arr = p.second;
        
        cout << arrName << '\n';
        for(int r = 0; r < arr.size(); r++){
            for(int c = 0; c < arr[0].size(); c++){
                cout << arr[r][c] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }

    return 1;
}