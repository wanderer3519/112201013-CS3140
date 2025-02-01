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
    void execerror(const char* s, const char* t);
    jmp_buf begin;

    void printAST(TreeNode* node) {
        if(!node) return;
        
        printAST(node->left);
        

        if (node->op)
            std::cout << node->op << ' ';
        else if (node->varIndex != -1)
            std::cout << "VAR[" << char('A' + node->varIndex) << "] ";
        else
            std::cout << node->value << ' ';
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
      NUMBER { $$ = new TreeNode($1); }
    | VAR { $$ = new TreeNode($1); }
    | VAR '=' expr { $$ = new TreeNode('=', new TreeNode($1), $3); }
    | expr '+' expr { $$ = new TreeNode('+', $1, $3); }
    | expr '-' expr { $$ = new TreeNode('-', $1, $3); }
    | expr '*' expr { $$ = new TreeNode('*', $1, $3); }
    | expr '/' expr { 
        if ($3->value == 0.0)
            execerror("division by zero", "");
        $$ = new TreeNode('/', $1, $3); 
    }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = new TreeNode('-', nullptr, $2); }
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

int main() {
    signal(SIGFPE, [](int) { execerror("floating point exception", NULL); });

    for (int i = 0; i < 26; i++) 
        mem[i] = 0.0; 

    if (setjmp(begin) != 0) {
        printf("Restarting...\n");
    }

    printf("Calculator started. Enter expressions:\n");
    yyparse();
    printf("Exiting calculator.\n");

    return 0;
}
