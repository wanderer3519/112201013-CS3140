#include "tree.hpp"
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

ofstream outputFileMips("./outputs/mips.txt");

#define cout outputFileMips


extern void yyerror(const char *s);
extern unordered_map<string, pair<int, vector<int>>> mem;

/**
 * Corrected MIPS code generation for FOR_STMT and other constructs.
 */


void generate_expr(TreeNode* root){
    if (!root)
		return;
	if (root->token == tokenVal){
        cout << "li $";
    }	


	else if (root->token == tokenVar && !mem.count(root->name))
	{
		
    }
	else if (root->token == tokenVar)
	{
	
	}

	else if (root->token == tokenArr)
	{
		
	}


	if (root->name == "+")
	{
		
	}
	else if (root->name == "-")
	{
		
	}
	else if (root->name == "*")
	{
		
	}
	else if (root->name == "/")
	{
		
	}
	else if (root->name == "%")
	{
		
	}
	else if (root->name == ">")
	{
		
	}
	else if (root->name == "<")
	{
		
	}
	else if (root->name == "==")
	{
		
	}
	else if (root->name == "!=")
	{
		
	}
	else if (root->name == ">=")
	{
		
	}
	else if (root->name == "<=")
	{
		
	}
	else
	{
		cout << root->name << '\n';
		
	}

	return;
}

void generate_for_stmt(TreeNode* root) {
    cout << "# MIPS code for FOR_STMT" << endl;

    // Initialization
    print_code_2(root->left->left); // Initialization statement

    cout << "FOR_START:" << endl;

    // Condition
    generate_expr(root->left->right); // Condition expression
    cout << "beqz $t0, FOR_END" << endl; // Exit loop if condition is false

    // Loop body
    print_code_2(root->right->right); // Loop body

    // Update statement
    print_code_2(root->right->left); // Update statement
    cout << "j FOR_START" << endl; // Jump back to start

    cout << "FOR_END:" << endl;
}

void generate_if_else(TreeNode* root) {
    cout << "# MIPS code for IF_ELSE" << endl;

    // Condition
    generate_expr(root->left); // Evaluate condition
    cout << "beqz $t0, ELSE_LABEL" << endl; // Branch to ELSE if condition is false

    // IF block
    print_code_2(root->right->left); // IF block
    cout << "j END_IF" << endl; // Skip ELSE block

    // ELSE block
    cout << "ELSE_LABEL:" << endl;
    print_code_2(root->right->right); // ELSE block

    cout << "END_IF:" << endl;
}

void generate_assignment(TreeNode* root) {
    cout << "# MIPS code for ASSIGNMENT" << endl;
    generate_expr(root->right); // Evaluate right-hand side
    cout << "sw $t0, " << root->left->numValue << "($sp)" << endl; // Store value in memory
}

void generate_write(TreeNode* root) {
    cout << "# MIPS code for WRITE" << endl;
    // print_code_2(root->left); // Evaluate expression
    generate_expr(root->right); // Evaluate expression

    cout << "li $v0, 1" << endl; // Load syscall for print integer
    cout << "move $a0, $t0" << endl; // Move result to $a0
    cout << "syscall" << endl;
}

void generate_break() {
    cout << "# MIPS code for BREAK" << endl;
    cout << "j FOR_END" << endl; // Jump to end of loop
}



void print_code_2(TreeNode* root) {
    if (!root)
        return;

    if (root->name == "WRITE") {
        generate_write(root);
    } else if (root->name == "=") {
        generate_assignment(root);
    } else if (root->name == "IF_ELSE") {
        generate_if_else(root);
    } else if (root->name == "FOR_STMT") {
        generate_for_stmt(root);
    } else if (root->name == "STMT_LIST") {
        print_code_2(root->left);
        print_code_2(root->right);
    } else if (root->name == "BREAK") {
        generate_break();
    } else {
        // Handle other cases or default behavior
        cout << "# Unknown node type: " << root->name << endl;
    }
}

void print_code(TreeNode* root){
    if (!root)
		return;


	/* Write every statement as a function then use them */

	if (root->name == "WRITE")
	{
		
	}

	else if (root->name == "=")
	{
		
	}

	else if (root->name == "IF_ELSE")
	{
		
	}
	else if (root->name == "FOR_STMT")
	{

	}
	else if (root->name == "STMT_LIST")
	{
		execute_stmt(root->left);
		// print_proper(root->left, "", true);
		execute_stmt(root->right);
	}
	else if (root->name == "BREAK")
	{
		
	}
}