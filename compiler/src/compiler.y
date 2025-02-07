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

	int yylex();
	void yyerror(const char* s);
    // int i;
	// Symbol* sym[1024];

	void preorder(TreeNode* root){
		if(!root) return;

		switch(root->type){
			case tokenVar:
			printf("VAR ");
			break;

			case tokenOp:
			printf("%s ", root->name);
			break;

			case tokenVal:
			printf("%s ", root->numValue);
			break;
		}

		preorder(root->left);
		preorder(root->right);
	}

%}

%union{
	int val;
	TreeNode* node;
}


/* %type <node> Gdecl_sec Gdecl_list */
%type <node> arg_list1 arg_list arg func ret_type Gid expr var_expr func_call param_list para param_list1 str_expr assign_stmt Gdecl_list

%token BEG END
%token <val> T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token <val> VAR NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token <val> T F 
%token MAIN RETURN


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
		printf("You are right\n");
		// preorder($2); // here
 	}
	;
	
Gdecl_list: 
	| 	Gdecl Gdecl_list
	;
	
/* gdecl: integer var1, var2, var3;
	ret_type: integer
	glist: var1, var2, var3;
	gid: var1 (VAR)
	gid: var2
	gid: var3
*/
Gdecl 	:	ret_type Glist ';'
	;
	
ret_type:	T_INT		{ $$ = new TreeNode($1, tokenKey); }
	;
	
Glist:	Gid
	| 	func 
	|	Gid ',' Glist 
	|	func ',' Glist
	;

Gid	:	VAR	{ $$ = new TreeNode($1, tokenVar); }
	|	Gid '[' NUM ']'	{ /* $$ = new TreeNode( "Index tokenOp,", new TreeNode($3)); */ }
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

func:	VAR '(' arg_list ')' { $$ = new TreeNode($1, tokenVar,  new TreeNode($1, tokenVar), $3); }
	;
		
arg_list:	
	|	arg_list1 { $$ = $1; }
	;
	
arg_list1:
    arg_list1 ',' arg { $$ = new TreeNode("ArgList", tokenKey,  $1, $3); }
    | arg { $$ = new TreeNode("ArgList", tokenKey, $1, nullptr); }
    ;

arg:
    expr { $$ = $1; }  // Argument is just an expression
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
		RETURN expr ';'	{ 					}
	;
		
MainBlock: 	
	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ 				  	  }			  
	| stmt_list
	;
	
main:	MAIN		{ 					}
	;
	
Ldecl_sec:	DECL Ldecl_list ENDDECL	{}
	;

Ldecl_list:		
	|	Ldecl Ldecl_list
	;

Ldecl:	
	type Lid_list ';'		
	;

type:	
	T_INT			{ }
	;

Lid_list:	
	Lid
	|	Lid ',' Lid_list';'
	;
	
Lid	:	
	VAR			{ 						}
	;


/* needed for assignment 2 */
stmt_list:	
		/* NULL */		{ /* maybe display the symbol table here */ }
	|	statement stmt_list	{						}
	|	error ';' 		{  }
	;

statement:	
	assign_stmt  ';'		{ 							 }
	|	read_stmt ';'		{ }
	|	write_stmt ';'		{ }
	|	cond_stmt 		{ }
	|	func_stmt ';'		{ }
	;

read_stmt:	
	READ '(' var_expr ')' {						 }
	;

write_stmt:	
	WRITE '(' expr ')' 	{  }
	|	WRITE '(''"' str_expr '"'')'      { }
	;

/* use this */
assign_stmt:	
	var_expr '=' expr 	{ 
		printf("Control Reached :)\n"); 
		$$ = new TreeNode("=", tokenOp, $1, $3);
	}
	;

cond_stmt:	
	IF expr THEN stmt_list ENDIF 	{ 						}
	|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ 						}
    |   FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'   {                                                 }
	;

func_stmt:	
	func_call 		{ 						}
	;
	
func_call:	VAR '(' param_list ')'	{ $$ = new TreeNode('C', tokenKey, new TreeNode($1, tokenVar), $3); }
	;
	
param_list:				
	|	param_list1		
	;
	
param_list1:	
	para			
	|	para ',' param_list1	
	;

para:	
	expr { $$ = $1; }
	;

expr:	
	NUM 			{ $$ = new TreeNode($1, tokenVal); }
	|	'-' NUM			{ $$ = new TreeNode( "-", tokenOp, nullptr, new TreeNode($2, tokenVal)); }
	|	var_expr		{ $$ = $1; }
	|	T			{ $$ = new TreeNode($1, tokenKey); }
	|	F			{ $$ = new TreeNode($1, tokenKey); }
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
	|	func_call		{ $$ = $1; }
	;

str_expr:
    VAR { $$ = new TreeNode($1, tokenVar); }  // Single variable node
  | str_expr VAR { $$ = new TreeNode("Concat", tokenVar, $1, new TreeNode($2, tokenVar)); }
  ;

var_expr:	
	VAR	{ $$ = new TreeNode($1, tokenVar); }
	|	var_expr '[' expr ']'	{ /* $$ = new TreeNode('V', $1, $3); */ }
	;
%%
void yyerror(const char* s){
	printf("Error is here\n");
   	fprintf (stderr, "%s\n", s);
}

int main(int argc, char* argv[]){
	/* FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

	yyin = file; */
	yyparse();
	/* fclose(file); */
}
