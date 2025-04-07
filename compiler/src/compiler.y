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
	#include <stdio.h>
	#include <stdlib.h>
	#include <bits/stdc++.h>
	
	using namespace std;

	unordered_map<string, pair<int, vector<int>> >mem;
	unordered_set<string> boolVars;
	// TreeNode* root;

	int yylex();
	void yyerror(const char* s);
	extern FILE* yyin;
    // int i;


	int lineno;
	
%}

%union{
	int val;
	const char* name;
	TreeNode* node;
	char endop;
}


/* %type <node> Gdecl_sec Gdecl_list */
%type <node> arg_list1 arg_list arg func ret_type 
%type <node> Gid expr var_expr func_call param_list para param_list1 str_expr assign_stmt Gdecl_list
%type <node> Gdecl Glist read_stmt write_stmt statement stmt_list func_stmt cond_stmt

%token BEG END
%token <val> T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token <val> NUM
%token <name> VAR
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token <val> T F 
%token MAIN RETURN
%token <name> STRING
%token BREAK


%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT
%%

Prog:	Gdecl_sec Fdef_sec MainBlock
	;

Gdecl_sec:	DECL Gdecl_list ENDDECL { 
		// printf("You are right\n");
		print_proper($2); // here
		declare_vars($2);
 	}
	;
	
Gdecl_list:  { $$ = nullptr; }
	| 	Gdecl Gdecl_list { 
			$$ = new TreeNode("DECL", tokenKey, $1, $2); 
		}
	;
	
/* gdecl: integer var1, var2, var3;
	ret_type: integer
	glist: var1, var2, var3;
	gid: var1 (VAR)
	gid: var2
	gid: var3
*/
Gdecl:	ret_type Glist  ';' {
			$$ = $1;
			$$->right = $2;
			// lineno++;
		}
		;
	
ret_type:	T_INT		{ 
		$$ = new TreeNode("INT", tokenKey); 
	}
	| T_BOOL {
		$$ = new TreeNode("BOOL", tokenKey);
	}
	;
	
Glist:	Gid { 
		$$ = $1; 
	}
	| 	func {  }
	|	Gid ',' Glist  { 
			$$ = $1;
			$$->right = $3;
		}
	|	func ',' Glist {  }
	;

Gid	:	VAR	{
			$$ = new TreeNode($1, tokenVar); 
			// if(!mem.count($1))
			// 	mem[$1] = {-1, {}};
			// else
			// 	yyerror("Redefined variable var");
		}
	|	VAR '[' NUM ']'	{
			$$ = new TreeNode("ARRAY", tokenArr, 
				 new TreeNode("X", tokenKey, new TreeNode($1, tokenVar),  new TreeNode($3, tokenVal)), nullptr);

			// if(!mem.count($1)){
			// 	mem[$1].first = 0;
			// 	mem[$1].second = vector<int>($3, -1);
			// }
			// else{
			// 	yyerror("Redefined variable array");
			// }
		}
	;
	
/* 
	func: read(integer a, integer b) 
	arg_list: integer a, integer b
	arg_list1: integer a, integer b
	arg: integer a
	arg: integer b

	var_list: a
	var_list: b
*/

func:	VAR '(' arg_list ')' { }
	;
		
arg_list: {  }	
	|	arg_list1 {  }
	;
	
arg_list1:
    arg_list1 ',' arg {  }
    | arg {  }
    ;

arg:
    expr {  }  // Argument is just an expression
    ;

/* arg_type:	T_INT		 { $$ = new TreeNode(tokenKey, $1); }
	; */

/* var_list:	
		VAR 		 { $$ = new TreeNode(new VarClass(sym[i++])); }
	|	VAR ',' var_list { $$ = new TreeNode(new VarClass(sym[i++]), nullptr, $3); }
	; */
	
Fdef_sec:	
	|	Fdef_sec Fdef
	;
	
Fdef:	
		func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{	 				}
	;
	
func_ret_type:	
		T_INT		{ } /* Only int is return type */
	;
		
func_name:	
		VAR		{ 					}
	;
	
FargList:	
		arg_list	{ 					}
	;
ret_stmt:	
		RETURN expr  ';'	{ 					}
	;
		
MainBlock: 	
	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ 				  	  }			  
	| BEG stmt_list END { 
			// root = $2;
			cout << '\n';
			print_proper($2);

			cout << "\nDone symbol table: Now executing the program...\n";
			cout << "Program Output:\n";
			execute_stmt($2);
		}
	;
	
main:	MAIN		{ 					}
	;
	
Ldecl_sec:	DECL Ldecl_list ENDDECL	{}
	;

Ldecl_list:		
	|	Ldecl Ldecl_list
	;

Ldecl:	
	type Lid_list  ';'		
	;

type:	
	T_INT			{ }
	;

Lid_list:	
	Lid
	|	Lid ',' Lid_list ';'
	;
	
Lid	:	
	VAR			{ 						}
	;


/* needed for assignment 2 */
stmt_list:	
		/* NULL */		{ $$ = nullptr; }
	|	statement stmt_list	{ $$ = new TreeNode("STMT_LIST", tokenKey, $1, $2); }
	|	error  ';' 		{  }
	;

statement:	
	assign_stmt   ';'		{ $$ = $1; }
	|	read_stmt  ';'		{ $$ = $1; }
	|	write_stmt  ';'		{ $$ = $1; }
	|	cond_stmt 			{ $$ = $1; }
	|	func_stmt  ';'		{ }
	| 	BREAK ';' { $$ = new TreeNode("BREAK", tokenKey); }
	;

read_stmt:	
	READ '(' var_expr ')' { 
			$$ = new TreeNode("READ", tokenKey, nullptr, $3); 
			// print_tree($$); 
		}
	;

write_stmt:	
	WRITE '(' expr ')' 	{ 
		$$ = new TreeNode("WRITE", tokenKey, nullptr, $3); 
		// print_tree($$);
		// execute_stmt($$);
	 }
	|	WRITE '(''"' str_expr '"'')'    { 
			// cout << "Here str_exrp\n";
			$$ = new TreeNode("WRITE", tokenKey, nullptr, $4); 
			// print_tree($$); 
			// execute_stmt($$);
		}
	| WRITE '(' STRING ')' { $$ = new TreeNode("WRITE", tokenKey, nullptr, new TreeNode($3, tokenStr)); }
	;

/* use this */
assign_stmt:  { $$ = nullptr; }
	| var_expr '=' expr 	{ 
		$$ = new TreeNode("=", tokenOp, $1, $3);
		// print_tree($$);
		// execute_stmt($$);
	}
	;

cond_stmt:	
	IF '(' expr ')' THEN stmt_list ENDIF 	{
			TreeNode* right = new TreeNode("Buf", tokenKey, $6, nullptr);
			
			$$ = new TreeNode("IF_ELSE", tokenKey, $3, right);
			// print_tree($$);
			
			
		}

	|	IF '(' expr ')' THEN stmt_list  ELSE  stmt_list  ENDIF	{ 
			TreeNode* right = new TreeNode("Buf", tokenKey, $6, $8);

			$$ = new TreeNode("IF_ELSE", tokenKey, $3, right); 
			
			// print_tree($$);
			// execute_stmt($$);
		}

    |   FOR '(' assign_stmt   ';'  expr  ';'  assign_stmt ')' '{' stmt_list '}'   { 
			$$ = new TreeNode("FOR_STMT", tokenKey);
			$$->left = new TreeNode("Buf1", tokenKey, $3, $5);
			$$->right = new TreeNode("Buf2", tokenKey, $7, $10);
			
			// print_tree($$);
			// execute_stmt($$);
	 	}
	;

func_stmt:	
	func_call 		{ $$ = $1; print_tree($$); }
	;
	
func_call:	VAR '(' param_list ')'	{ $$ = new TreeNode("CALL", tokenKey, new TreeNode($1, tokenVar), $3); }
	;
	
param_list:	{ $$ = nullptr; }
	|	param_list1	{ $$ = $1; }
	;
	
param_list1:	
	para { $$ = $1; }
	|	para ',' param_list1 { 
		$$ = $1;
		$$->right = $3; 
	}	
	;

para:	
	expr { $$ = $1; }
	;

expr:	
	NUM 			{ $$ = new TreeNode($1, tokenVal); }
	|	'-' NUM			{ $$ = new TreeNode( "-", tokenOp, nullptr, new TreeNode($2, tokenVal)); }
	|	var_expr		{ $$ = $1; }
	|	T			{ $$ = new TreeNode($1, tokenVal); }
	|	F			{ $$ = new TreeNode($1, tokenVal); }
	|	'(' expr ')'		{ $$ = $2; }
	|	expr '+' expr 		{ $$ = new TreeNode( "+", tokenOp, $1, $3); }
	|	expr '-' expr	 	{ $$ = new TreeNode( "-", tokenOp, $1, $3); }
	|	expr '*' expr 		{ $$ = new TreeNode( "*", tokenOp, $1, $3); }
	|	expr '/' expr 		{ $$ = new TreeNode( "/", tokenOp, $1, $3); }
	|	expr '%' expr 		{ $$ = new TreeNode( "%", tokenOp, $1, $3); }
	|	expr '<' expr		{ $$ = new TreeNode( "<", tokenOp, $1, $3); }
	|	expr '>' expr		{ $$ = new TreeNode( ">", tokenOp, $1, $3); }
	|	expr GREATERTHANOREQUAL expr	{ $$ = new TreeNode( ">=", tokenOp, $1, $3);		}
	|	expr LESSTHANOREQUAL expr		{ $$ = new TreeNode( "<=", tokenOp, $1, $3); 	}
	|	expr NOTEQUAL expr				{ $$ = new TreeNode( "!=", tokenOp, $1, $3);		}
	|	expr EQUALEQUAL expr			{ $$ = new TreeNode( "==", tokenOp, $1, $3);		}
	|	LOGICAL_NOT expr				{ $$ = new TreeNode( "!", tokenOp, nullptr, $2);	}
	|	expr LOGICAL_AND expr			{ $$ = new TreeNode( "&&", tokenOp, $1, $3);		}
	|	expr LOGICAL_OR expr			{ $$ = new TreeNode( "||", tokenOp, $1, $3);		}
	|	func_call		{ /* $$ = $1; */ }
	;

str_expr:
    VAR { 
			$$ = new TreeNode("STRING", tokenKey, new TreeNode($1, tokenStr), nullptr); 
		}  // Single VAR node
  | str_expr VAR { 
			string name = $1->left->name;
			name += ' ';
			for(auto c: name){
				name.push_back(c);
			}
			$$->left->name = name;
		}
  ;

var_expr:	
	VAR	{ $$ = new TreeNode($1, tokenVar); }
	|	var_expr '[' expr ']'	{ $$ = new TreeNode("ARRAY", tokenArr, $1, $3); }
	;
%%

void yyerror(const char* s){
	print_symbol_table(mem);
   	fprintf(stderr, "Error: %s at line number: %d \n", s, lineno);
}


int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Use this cmd:\n$ %s <file_path>\n", argv[0]);
		return 0;
	}

	FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

	yyin = file;

	yyparse();
	
	fclose(file);
	return 0;
}
