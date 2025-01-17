/* 
    Conventional combination lex and yacc files for the calculator.
    To run this use

    $ make
    $ ./calculator
 */

%{
    #include <stdio.h>
    #include <ctype.h>
    #include <signal.h>
    #include <setjmp.h>

    // Memory for variables A-Z
    double mem[26]; 

    // yylex(): Done by lex file
    int yylex();

    // For error handling: Not fully tested
    void yyerror(const char* s);

    jmp_buf begin;
%}

/* Using a union for yylval in order to add the functionality of variables */
%union {
    double val;
    int index;
}

/* Tokens: Number and Variables */
%token <val> NUMBER
%token <index> VAR

%type <val> expr

%left '+' '-'
%left '*' '/'
%left UNARYMINUS

%%
/* Grammar for list */
list:
    /* epsilon */
    | list '\n'
    | list expr '\n' { printf("\tResult: %.8g\n", $2); }
    ;

/* Grammar for expression */
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

/* Error handling: Not fully tested */
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

/* Main: Code starts here */
int main() {
    // To handle floating-point exceptions
    signal(SIGFPE, fpecatch); 

    // A good practice to initialize variable memory
    for (int i = 0; i < 26; i++) 
        mem[i] = 0.0; 
    
    // Restarting when an error occurs
    if (setjmp(begin) != 0) {
        printf("Restarting...\n");
    }

    // Parsing: Done by lex
    yyparse();
    return 0;
}


