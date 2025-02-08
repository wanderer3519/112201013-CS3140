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
			printf("%d ", root->numValue);
			break;

			case tokenKey:
			printf("%s ", root->name);
			break;
		}

		preorder(root->left);
		preorder(root->right);
	}

	/*	Helper function to print one level of a binary tree */
	void print_level(TreeNode* root, int t_level, int p_level, int height) {
	/*
		Inputs:
			1.	Top level node t
			2.	Height of the top level node (t) in the whole binary tree
			3.	Level of the binary tree to be printed
			4.	Height of the whole binary tree
	*/
		if(t_level > p_level) {
			fprintf(stderr, "Error in print_level\n");
			return;
			}
		if(t_level < p_level) {
			if (root != NULL)
				print_level(root->left, t_level+1, p_level, height);
			else
				print_level(NULL, t_level+1, p_level, height);

			if (root != NULL)
				print_level(root->right, t_level+1, p_level, height);
			else
				print_level(NULL, t_level+1, p_level, height);
			return;
		}

		// t_level == p_level
		int padding = 1;	// for alignment
		for (int i = p_level; i < height-1; i++)
			padding = padding * 2 + 1;

		// To prefix some space and a line 
		for (int i = 0; i < (padding+1)/2; i++)
			printf("  ");
		for (int i = (padding+1)/2; i < padding; i++)
			printf("__");

		if (root != NULL){
			switch(root->type){
				case tokenVar:
				printf("VAR ");
				break;

				case tokenOp:
				printf("%4s ", root->name);
				break;

				case tokenVal:
				printf("%4d ", root->numValue);
				break;

				case tokenKey:
				printf("%4s ", root->name);
				break;
			}
		}
		else
			printf("()");	

		// To suffix a line and some space
		for (int i = (padding+1)/2; i < padding; i++)
			printf("__");
		for (int i = 0; i < (padding+1)/2; i++)
			printf("  ");

		printf("  ");
		return;
	}

	int height(TreeNode* root){
		if(!root) return 0;

		int left = height(root->left);
		int right = height(root->right);

		return 1 + max(left, right);
	}

	/*	Helper function to print the binary tree in a tree-like way. */
	void print_tree(TreeNode* t) {
		int h = height(t);
		for(int l = 0; l < h; l++) {
			print_level(t, 0, l, h);
			printf("\n");
			}
		return;
	}

	

%}

%union{
	int val;
	TreeNode* node;
}


/* %type <node> Gdecl_sec Gdecl_list */
%type <node> arg_list1 arg_list arg func ret_type 
%type <node> Gid expr var_expr func_call param_list para param_list1 str_expr assign_stmt Gdecl_list
%type <node> Gdecl Glist read_stmt write_stmt statement stmt_list func_stmt

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
		// printf("You are right\n");
		print_tree($2); // here
 	}
	;
	
Gdecl_list:  { $$ = nullptr; }
	| 	Gdecl Gdecl_list { $$ = new TreeNode("DECL", tokenKey, $2, $1); }
	;
	
/* gdecl: integer var1, var2, var3;
	ret_type: integer
	glist: var1, var2, var3;
	gid: var1 (VAR)
	gid: var2
	gid: var3
*/
Gdecl 	:	ret_type Glist ';' { /* $$ = new TreeNode("Key", tokenKey, $1, $2); */ 
		$$ = $1;
		$$->right = $2;
	}
	;
	
ret_type:	T_INT		{ 
		/* printf("%d\n", $1); */
		$$ = new TreeNode("INT", tokenKey); 
	}
	;
	
Glist:	Gid { $$ = new TreeNode("VAR", tokenVar); }
	| 	func { /* $$ = $1; */ }
	|	Gid ',' Glist  { 
		// printf("Using\n");
			$$ = $1;
			$$->right = $3;
		}
	|	func ',' Glist { /* $$ = new TreeNode("ARGS", tokenKey, $1, $3); */ }
	;

Gid	:	VAR	{ printf("VAR: %d\n", $1); $$ = new TreeNode($1, tokenVar); }
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

func:	VAR '(' arg_list ')' { printf("VAR: %d\n", $1); $$ = new TreeNode($1, tokenVar,  new TreeNode($1, tokenVar), $3); }
	;
		
arg_list: { $$ = nullptr; }	
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
	|	statement stmt_list	{ }
	|	error ';' 		{  }
	;

statement:	
	assign_stmt  ';'		{ $$ = $1; }
	|	read_stmt ';'		{ $$ = $1; }
	|	write_stmt ';'		{ $$ = $1; }
	|	cond_stmt 			{ }
	|	func_stmt ';'		{ }
	;

read_stmt:	
	READ '(' var_expr ')' { $$ = new TreeNode("READ", tokenKey, nullptr, $3); print_tree($$); }
	;

write_stmt:	
	WRITE '(' expr ')' 					{ $$ = new TreeNode("WRITE", tokenKey, nullptr, $3); print_tree($$); }
	|	WRITE '(''"' str_expr '"'')'    { $$ = new TreeNode("WRITE", tokenKey, nullptr, $4); print_tree($$); }
	;

/* use this */
assign_stmt:	
	var_expr '=' expr 	{ 
		/* printf("Control Reached :)\n");  */
		$$ = new TreeNode("=", tokenOp, $1, $3);
		print_tree($$);
	}
	;

cond_stmt:	
	IF expr THEN stmt_list ENDIF 	{ 						}
	|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ 						}
    |   FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'   {                                                 }
	;

func_stmt:	
	func_call 		{ $$ = $1; print_tree($$); }
	;
	
func_call:	VAR '(' param_list ')'	{ printf("VAR: %d\n", $1); $$ = new TreeNode("CALL", tokenKey, new TreeNode($1, tokenVar), $3); }
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
	|	func_call		{ /* $$ = $1; */ }
	;

str_expr:
    VAR { printf("VAR: %d\n", $1); $$ = new TreeNode($1, tokenVar); }  // Single variable node
  | str_expr VAR { printf("VAR: %d\n", $1); $$ = new TreeNode("Concat", tokenVar, $1, new TreeNode($2, tokenVar)); }
  ;

var_expr:	
	VAR	{ printf("VAR: %d\n", $1); $$ = new TreeNode($1, tokenVar); }
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
