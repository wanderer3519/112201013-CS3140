# SIL Language Syntax Specification

This document describes the formal grammar and syntax rules of the **SIL (Simple Intermediate Language)** used in the SIL Compiler.

## Program Structure

```
Prog        ::= Gdecl_sec Fdef_sec MainBlock

Gdecl_sec   ::= 'DECL' Gdecl_list 'ENDDECL'
Gdecl_list  ::= ε | Gdecl Gdecl_list

Gdecl       ::= ret_type Glist ';'
ret_type    ::= 'int' | 'bool'

Glist       ::= Gid
              | Gid ',' Glist
              | func
              | func ',' Glist

Gid         ::= VAR
              | VAR '[' NUM ']'
```

## Declarations

``` 
Gdecl_sec   ::= 'DECL' Gdecl_list 'ENDDECL'
Gdecl_list  ::= ε | Gdecl Gdecl_list

Gdecl       ::= ret_type Glist ';'
ret_type    ::= 'int' | 'bool'

Glist       ::= Gid
              | Gid ',' Glist
              | func
              | func ',' Glist

Gid         ::= VAR
              | VAR '[' NUM ']'
```

## Functions

```
Fdef_sec    ::= ε | Fdef_sec Fdef

Fdef        ::= func_ret_type func_name '(' FargList ')' 
                '{' Ldecl_sec 'begin' stmt_list ret_stmt 'end' '}'

func_ret_type ::= 'int'
func_name     ::= VAR
FargList      ::= arg_list
arg_list      ::= ε | arg_list1
arg_list1     ::= arg | arg_list1 ',' arg
arg           ::= expr
ret_stmt      ::= 'return' expr ';'
```

## Main block

```
MainBlock   ::= func_ret_type 'main' '(' ')' 
                '{' Ldecl_sec 'begin' stmt_list ret_stmt 'end' '}'
              | 'begin' stmt_list 'end'
```

## Local declarations

```
Ldecl_sec   ::= 'DECL' Ldecl_list 'ENDDECL'
Ldecl_list  ::= ε | Ldecl Ldecl_list
Ldecl       ::= type Lid_list ';'
type        ::= 'int'
Lid_list    ::= Lid | Lid ',' Lid_list
Lid         ::= VAR
```

## Statements

```
stmt_list   ::= ε 
              | statement stmt_list
              | error ';'

statement   ::= assign_stmt ';'
              | read_stmt ';'
              | write_stmt ';'
              | cond_stmt
              | func_stmt ';'
              | 'break' ';'
```

## Assignment statement

```
assign_stmt ::= var_expr '=' expr
              | var_expr '++'
              | var_expr '--'
              | var_expr '+=' expr
              | var_expr '-=' expr
```

## Read write statement

```
read_stmt   ::= 'read' '(' var_expr_list ')'

write_stmt  ::= 'write' '(' expr_list ')'
              | 'write' '(' '"' str_expr '"' ')'
              | 'write' '(' STRING ')'
```

## Conditional statement

```
cond_stmt   ::= 'if' '(' expr ')' 'then' stmt_list 'endif'
              | 'if' '(' expr ')' 'then' stmt_list 'else' stmt_list 'endif'
              | 'for' '(' assign_stmt ';' expr ';' assign_stmt ')' 
                   '{' stmt_list '}'
```

## Function calls

```
func_stmt   ::= func_call
func_call   ::= VAR '(' param_list ')'
param_list  ::= ε | param_list1
param_list1 ::= para | para ',' param_list1
para        ::= expr
```

## Expressions

```
expr        ::= NUM
              | '-' NUM
              | var_expr
              | 'true' | 'false'
              | '(' expr ')'
              | expr '+' expr
              | expr '-' expr
              | expr '*' expr
              | expr '/' expr
              | expr '%' expr
              | expr '<' expr
              | expr '>' expr
              | expr '<=' expr
              | expr '>=' expr
              | expr '==' expr
              | expr '!=' expr
              | '!' expr
              | expr '&&' expr
              | expr '||' expr
              | func_call
```

## Variables

```
var_expr        ::= VAR
                  | var_expr '[' expr ']'

var_expr_list   ::= var_expr 
                  | var_expr ',' var_expr_list

expr_list       ::= expr 
                  | expr ',' expr_list
```

## Strings

```
str_expr    ::= VAR
              | str_expr VAR
```



# Sample program

``` sil []
decl
    int x, y, arr[10];
enddecl

begin
    read(x);
    y = x + 5;
    if (y > 10) then
        write("Greater than 10");
    else
        write("Not greater than 10");
    endif;
end
```

## NOTE
- Functions are not currently suppoted in this version of the compiler
- Implementing functions in SIL will be a future work