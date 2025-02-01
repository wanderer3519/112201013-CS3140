/* 
    This is a version of the code entirely written in yacc file
    I have pushed it just for reference.

    I also have pushed a yacc, lex files seperately in the required format
    To run this use
    
    $ make hoc 
    $ ./hoc1
 */

%{
    #include "hoc.h"
    #include <stdio.h>
    #include <ctype.h>
    #include <signal.h>
    #include <setjmp.h>

    extern double Pow();

    double mem[26];
%}

%union{
    double val;
    Symbol* sym;
}

%token <val> NUMBER
%token <sym> VAR BLTIN UNDEF
%type <val> expr asgn
%right '='
%left '+' '-'
%left '*' '/'
%left UNARYMINUS
%right '^'

%%
list: // epsilon
    | list '\n'
    | list asgn '\n'
    | list expr '\n' { printf("\tResult %.8g\n", $2); }
    | list error '\n' { yyerrok; }
    ;

asgn: VAR '=' expr { 
    $$ = $1->u.val =$3; 
    $1->type = VAR;
}

expr: NUMBER 
    | VAR { 
        if($1->type == UNDEF)
            execerror("undefined variable", $1->name);
        $$ = $1->u.val;
    }
    | asgn
    | BLTIN '(' expr ')' { $$ = (*($1->u.ptr))($3); }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if($3 == 0.0)
            execerror("division by zero", "");
        $$ = $1 / $3; 
    }
    | expr '^' expr { $$ = Pow($1, $3); }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = -$2; }
    ;
%%


jmp_buf begin;

char* progname;
int lineno = 1;

int fpecatch(){
    execerror("floating point exception", (char*) 0);
}

int execerror(char* s, char* t){
    warning(s, t);
    longjmp(begin, 0);
}

int yylex(){
    int c;

    while((c = getchar()) == ' ' || c == '\t');

    if(c == EOF) return 0;

    if(c == '.' || isdigit(c)){
        ungetc(c, stdin);
        scanf("%lf", &yylval.val);
        return NUMBER;
    }

    if(isupper(c)){
        yylval.index = c - 'A';
        return VAR;
    }

    if(c == '\n') lineno++;

    if(isalpha(c)){
        Symbol* s;
        char sbuf[1024];
        char* p = sbuf;

        do{
            *p++ = c;
        }while((c = getchar()) != EOF && isalnum(c));
    
        ungetc(c, stdin);
        *p = '\0';
        if((s = lookup(sbuf)) == 0)
            s = install(sbuf, UNDEF, 0.0);
        yylval.sym = s;

        return s->type == UNDEF ? VAR : s->type;
    }

    return c;
}

int warning(char* s, char* t){
    fprintf(stderr, "%s: %s", progname, s);
    if(t)
        fprintf(stderr, " %s", t);
    
    fprintf(stderr, " near line %d\n", lineno);

    return 0;
}

int yyerror(char* s){
    warning(s, (char*) 0);

    return 0;
}

int main(int argc, char* argv[]){
    progname = argv[0];
    init();
    setjmp(begin);
    signal(SIGFPE, fpecatch);
    yyparse();

    return 0;
}
