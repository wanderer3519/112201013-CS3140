%{
    #include <stdio.h>
    #include <ctype.h>
    #include <signal.h>
    #include <setjmp.h>
    #include "calculator.hpp"

    // Memory for variables A-Z
    double mem[26]; 

    // Function prototypes
    int yylex();
    void yyerror(const char* s);
    void warning(const char* s, const char* t);
    void execerror(const char* s, const char* t);    extern FILE* yyin;
    jmp_buf begin;

    void printAST(TreeNode* node) {
        if(!node) return;
        
        printAST(node->left);
        
        if(node->type == tokenOp){
            printf("%c ", node->value);
        }
        else if(node->type == tokenVar){
            printf("VAR ");
        }
        else printf("%d ", node->value);

        printAST(node->right);
    }

%}

/* Define token types manually for Lex */
%union {
    int val;
    int index;
    TreeNode* Node;
}

%token <val> NUMBER
%token <index> VAR

%type <Node> expr

%left '+' '-'
%left '*' '/'
%right UNARYMINUS

%%
list:
    /* empty */
    | list '\n'
    | list expr '\n' { 
        printf("Parsed expression\n");
        printAST($2); 
        printf("\n");
    }
    ;

expr:
      NUMBER { $$ = new TreeNode(tokenNum, $1); }
    | VAR { $$ = new TreeNode(tokenVar, $1); }
    | VAR '=' expr { $$ = new TreeNode(tokenOp, '=', new TreeNode(tokenVar, $1), $3); }
    | expr '+' expr { $$ = new TreeNode(tokenOp, '+', $1, $3); }
    | expr '-' expr { $$ = new TreeNode(tokenOp, '-', $1, $3); }
    | expr '*' expr { $$ = new TreeNode(tokenOp, '*', $1, $3); }
    | expr '/' expr { 
        if ($3->value == 0.0)
            execerror("division by zero", "");
        $$ = new TreeNode(tokenOp, '/', $1, $3); 
    }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = new TreeNode(tokenOp, '-', nullptr, $2); }
    ;

%%

void warning(const char* s, const char* t) {
    fprintf(stderr, "Error: %s", s);
    if (t) fprintf(stderr, " %s", t);
    fprintf(stderr, "\n");
}

void execerror(const char* s, const char* t) {
    warning(s, t);
    longjmp(begin, 1);
}

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
}

/* Main: Code starts here */
int main(int argc, char* argv[]) {
    // To handle floating-point exceptions
    signal(SIGFPE, fpecatch); 

    for (int i = 0; i < 26; i++) 
        mem[i] = 0.0; 

    if (setjmp(begin) != 0) {
        printf("Restarting...\n");
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

    yyin = file;

    // Parsing: Done by lex
    yyparse();
    printf("Exiting calculator.\n");

    fclose(file);
    return 0;
}
