%{
    #include <stdio.h>
    #include <ctype.h>
    #include <signal.h>
    #include <setjmp.h>

    double mem[26]; // Memory for variables A-Z

    int yylex();
    void yyerror(const char* s);

    jmp_buf begin;
%}

%union {
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
list:
    /* epsilon */
    | list '\n'
    | list expr '\n' { printf("\tResult: %.8g\n", $2); }
    ;

expr:
      NUMBER { $$ = $1; }
    | VAR { $$ = mem[$1]; }
    | VAR '=' expr { $$ = mem[$1] = $3; }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if ($3 == 0.0)
            execerror("division by zero", "");
        $$ = $1 / $3; 
    }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = -$2; }
    ;

%%

void execerror(const char* s, const char* t) {
    warning(s, t);
    longjmp(begin, 1);
}

void warning(const char* s, const char* t) {
    fprintf(stderr, "Error: %s", s);
    if (t)
        fprintf(stderr, " %s", t);
    fprintf(stderr, "\n");
}

void yyerror(const char* s) {
    warning(s, NULL);
}

void fpecatch(int signum) {
    execerror("floating point exception", NULL);
}

int main() {
    signal(SIGFPE, fpecatch); // Handle floating-point exceptions
    for (int i = 0; i < 26; i++) mem[i] = 0.0; // Initialize variable memory
    if (setjmp(begin) != 0) {
        printf("Restarting...\n");
    }
    yyparse();
    return 0;
}


