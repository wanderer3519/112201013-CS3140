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
	#include <stdio.h>
	int yylex();
	void yyerror(char*);
    int i;
%}

%union{
	Node* node;
	int val;
	Func* fun; 
};


/* %type <node> Gdecl_sec Gdecl_list */
%type <node> Gid ret_type
%type <fun> func

%token BEG END
%token T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token VAR NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token T F 
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
		
	Gdecl_sec:	DECL Gdecl_list ENDDECL { }
		;
		
	Gdecl_list: 
		| 	Gdecl Gdecl_list
		;
		
	/* gdecl: integer var1, var2, var3;
		ret_type: integer
		glist: var1, var2, var3;
		gid: 
	*/
	Gdecl 	:	ret_type Glist ';'
		;
		
	ret_type:	T_INT		{ $$ = new Node($1, T_INT); }
		;
		
	Glist 	:	Gid
		| 	func 
		|	Gid ',' Glist 
		|	func ',' Glist
		;
	
	Gid	:	VAR	{ $$ = new Node($1, VAR); }
		|	Gid '[' NUM ']'	{ $$ = $1; }
		;
		
	func:	VAR '(' arg_list ')' { $$ = Func(); }
		;
			
	arg_list:	
		|	arg_list1
		;
		
	arg_list1:	arg_list1 ';' arg
		|	arg
		;
		
	arg 	:	arg_type var_list	
		;
		
	arg_type:	T_INT		 {  }
		;

	var_list:	VAR 		 { }
		|	VAR ',' var_list { 	}
		;
		
	Fdef_sec:	
		|	Fdef_sec Fdef
		;
		
	Fdef	:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{	 				}
		;
		
	func_ret_type:	T_INT		{ }
		;
			
	func_name:	VAR		{ 					}
		;
		
	FargList:	arg_list	{ 					}
		;

	ret_stmt:	RETURN expr ';'	{ 					}
		;
			
	MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ 				  	  }
					  
		;
		
	main	:	MAIN		{ 					}
		;
		
	Ldecl_sec:	DECL Ldecl_list ENDDECL	{}
		;

	Ldecl_list:		
		|	Ldecl Ldecl_list
		;

	Ldecl	:	type Lid_list ';'		
		;

	type	:	T_INT			{ }
		;

	Lid_list:	Lid
		|	Lid ',' Lid_list';'
		;
		
	Lid	:	VAR			{ 						}
		;

	/* needed for assignment 2 */
	stmt_list:	/* NULL */		{  }
		|	statement stmt_list	{						}
		|	error ';' 		{  }
		;

	statement:	assign_stmt  ';'		{ 							 }
		|	read_stmt ';'		{ }
		|	write_stmt ';'		{ }
		|	cond_stmt 		{ }
		|	func_stmt ';'		{ }
		;

	read_stmt:	READ '(' var_expr ')' {						 }
		;

	write_stmt:	WRITE '(' expr ')' 	{  }
		 | WRITE '(''"' str_expr '"'')'      { }

		;
	
	assign_stmt:	var_expr '=' expr 	{ 						}
		;

	cond_stmt:	IF expr THEN stmt_list ENDIF 	{ 						}
		|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ 						}
	        |    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'                                             {                                                 }
		;
	
	func_stmt:	func_call 		{ 						}
		;
		
	func_call:	VAR '(' param_list ')'	{ 						   }
		;
		
	param_list:				
		|	param_list1		
		;
		
	param_list1:	para			
		|	para ',' param_list1	
		;

	para	:	expr			{ 						}
		;

	expr	:	NUM 			{ 			}
		|	'-' NUM			{  						   }
		|	var_expr		{ 			}
		|	T			{ 						  	}
		|	F			{ 	}
		|	'(' expr ')'		{  			}

		|	expr '+' expr 		{ 						}
		|	expr '-' expr	 	{ 						}
		|	expr '*' expr 		{ 			}
		|	expr '/' expr 		{ 					}
		|	expr '%' expr 		{ 						}
		|	expr '<' expr		{ 						}
		|	expr '>' expr		{ 						}
		|	expr GREATERTHANOREQUAL expr				{ 						}
		|	expr LESSTHANOREQUAL expr	{  						}
		|	expr NOTEQUAL expr			{ 						}
		|	expr EQUALEQUAL expr	{ 						}
		|	LOGICAL_NOT expr	{ 						}
		|	expr LOGICAL_AND expr	{ 						}
		|	expr LOGICAL_OR expr	{ 						}
		|	func_call		{  }

		;
	str_expr :  VAR                       {}
                  | str_expr VAR   { }
                ;
	
	var_expr:	VAR	{ 				  }
		|	var_expr '[' expr ']'	{                                                 }
		;
%%
void yyerror(char* s){
   fprintf (stderr, "%s\n", s);
}

int main(int argc, char* argv[]){
	FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

	yyin = file;
	yyparse();
	fclose(file);
}
