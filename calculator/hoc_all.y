%{
    #include <stdio.h>
    #include <ctype.h>
    #include <signal.h>
    #include <setjmp.h>

    double mem[26];
%}

%union{
    double val;
    int index;
}

%token <val> NUMBER
%token <index> VAR
%type <val> expr
%left '+' '-'
%left '*' '/'
%left UNARYMINUS

%%
list: // epsilon
    | list '\n'
    | list expr '\n' { printf("\tResult %.8g\n", $2); };

expr: NUMBER { $$ = $1; }
    | VAR { $$ = mem[$1]; }
    | VAR '=' expr { $$ = mem[$1] = $3; }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if($3 == 0.0)
            execerror("division by zero", "");
        $$ = $1 / $3; 
    }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = -$2; };
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

int main(int argc, char* argv[]){
    progname = argv[0];
    setjmp(begin);
    signal(SIGFPE, fpecatch);
    yyparse();

    return 0;
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