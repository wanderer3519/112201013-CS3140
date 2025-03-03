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
        
        case '>': ans = left_eval > right_eval; break;
        case '<': ans = left_eval < right_eval; break;
	}
	return ans;
}

void execute_stmt(TreeNode* root){
    if(!root) return;


    // cout << "DEBUG: " << root->name << '\n';

    /* Write every statement as a function then use them */
    
    if(root->name == "WRITE"){
        // $3 = root->right
        // string printing
        if(root->right->name == "STRING")
            cout << root->right->left->name << '\n';
        
        // expr printing 
        else {
            
            if(root->right->token == tokenVar){
                cout << mem[root->right->name].first << '\n';
            }
            else if(root->right->token == tokenVal) {
                int eval = evaluate_expr(root->right);
                cout << eval << '\n';
            }
            else if(root->right->token == tokenArr){
                string ind = root->right->left->name;
                int val = evaluate_expr(root->right->right);
                cout << mem[ind].second[val] << '\n'; 
            }
        } 
        
    }

    else if(root->name == "="){
        if(root->left->token == tokenVar && !mem.count(root->left->name)){
			yyerror("Undefined variable in assign");
			exit(0);
		} 

		if(root->left->token == tokenArr && !mem.count(root->left->left->name)){
			yyerror("Undefined variable in assign");
			exit(0);
		}
		
		int x = evaluate_expr(root->right);

		if(root->left->token == tokenVar)
			mem[root->left->name].first = x;
		
		else if(root->left->token == tokenArr) {
			int ind = evaluate_expr(root->left->right);
			string name = root->left->left->name;

			// cout << mem[name].second[0] << '\n';
			mem[name].second[ind] = x;
		}
    }

    else if(root->name == "IF_ELSE") {
        int exp = evaluate_expr(root->left);
		if(exp){
			// curr is the stmt_list
			TreeNode* curr = root->right->left;
			cout << curr->name << '\n';
			while(curr){
				execute_stmt(curr->left);
				curr = curr->right;
			}
		}

        else{
            // curr is the stmt_list
            TreeNode* curr = root->right->right;
            
            while(curr){
                execute_stmt(curr->left);
                curr = curr->right;
            }
        }
    }
	else if(root->name == "FOR_STMT"){
		execute_stmt(root->left->left); // execute assign
		
		TreeNode* cond = root->left->right;
		int flag = evaluate_expr(cond);
		while(flag){
			// cout << "Hello1\n";
			TreeNode* stlist = root->right->right;

			while(stlist){
				execute_stmt(stlist->left);
				stlist = stlist->right;
			}

			execute_stmt(root->right->left); // execute assign
			cond = root->left->right;
			flag = evaluate_expr(cond);
		}
	}
}
