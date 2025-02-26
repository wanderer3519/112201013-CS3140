#ifndef COMPILER_CPP
#define COMPILER_CPP

#include "tree.hpp"
#include <bits/stdc++.h>
using namespace std;

extern void yyerror(const char* s);
extern unordered_map<string, pair<int, vector<int>> >mem;

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
		switch(root->token){
			// case tokenVar:
			// // printf("%s", root->name);
			// cout << root->name;
			// break;
			case tokenOp:
			// printf("%s", root->name);
				if(root->name[0] == '+'){
					printf("PLUS");
				}
				else if(root->name[0] == '-'){
					printf("MINUS");
				}
				else if(root->name[0] == '*'){
					printf("MULT");
				}
				else if(root->name[0] == '/'){
					printf("DIV");
				}
				else if(root->name[0] == '='){
					printf("ASSIGN");
				}
				else{
					cout << root->name;
				}
			break;
			case tokenVal:
			printf("%d", root->numValue);
			break;
			// case tokenArr:
			// // printf("%s", root->name);
			// cout << root->name;
			// break;
			// case tokenKey:
			// // printf("%s", root->name);
			// cout << root->name;
			// break;
			// case tokenStr:
			// // printf("%s", root->name);
			// cout << root->name;
			// break;
			default:
				cout << root->name;
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

int evaluate_expr(TreeNode* root){
	if(!root) return 0;
	if(root->token == tokenVal)
		return root->numValue;
	else if(root->token == tokenVar && !mem.count(root->name)){
		// printf("VAR: %s|\n", root->name);
		// char* str;
		// sprintf(str, "Undefined Variable %s", root->name);
		yyerror("Undefined Variable in Eval expr");
		exit(0);
	}
	else if(root->token == tokenVar){
		return mem[root->name].first;
	}
	int left_eval = evaluate_expr(root->left);
	int right_eval = evaluate_expr(root->right);
	int ans = 0;
	switch(root->name[0]){
		case '+': ans = left_eval + right_eval; break;
		case '-': ans = left_eval - right_eval; break;
		case '*': ans = left_eval * right_eval; break;
		case '/':
			if(right_eval == 0){
				// char* str = "Division by zero";
				yyerror("Division by zero");
				exit(0);
			} 
			ans = left_eval / right_eval;
		break;
	}
	return ans;
}

void execute_stmt(TreeNode* root){

}

#endif