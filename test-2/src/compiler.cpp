#include "tree.hpp"
#include <bits/stdc++.h>
using namespace std;

extern void yyerror(const char* s);
extern unordered_map<string, pair<int, vector<int>> >mem;
extern unordered_map<string, pair<float, vector<float>> >fmem;

extern unordered_map< string, vector< vector<int> > > mem2d;
extern unordered_map< string, vector< vector<float> >> fmem2d;

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

			case tokenFloat:
			printf("%.2f", root->floatValue);
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



float evaluate_expr(TreeNode* root) {
    if (!root) return 0;
    
    // Return value if it's a number
    if (root->token == tokenVal) return root->numValue;
    if (root->token == tokenFloat) return root->floatValue;

    // Check if variable is undefined
    if (((root->token == tokenVar) 
        && !mem.count(root->name) && !fmem.count(root->name))
		|| (root->token == token2d && !mem2d.count(root->left->name) && !fmem2d.count(root->left->name))
		) {
        // cout << "NAME: " << root->name << '\n';
		yyerror("Undefined Variable in Eval expr");
        exit(1);
    }

    // Variable evaluation
    if (root->token == tokenVar) {
        if (mem.count(root->name)) return mem[root->name].first;
        if (fmem.count(root->name)) return fmem[root->name].first;
    }

    // 1D Array evaluation
    if (root->token == tokenArr) {
        int x = (int)evaluate_expr(root->right);
        
        if (mem.count(root->left->name)) {
            if (x < 0 || x >= mem[root->left->name].second.size()) {
                yyerror("Array index out of bounds");
                exit(1);
            }
            return mem[root->left->name].second[x];
        }
        else if (fmem.count(root->left->name)) {
            if (x < 0 || x >= fmem[root->left->name].second.size()) {
                yyerror("Array index out of bounds");
                exit(1);
            }
            return fmem[root->left->name].second[x];
        }
    }

    // 2D Array evaluation
    if (root->token == token2d) {
        int x = (int)evaluate_expr(root->right->left);
        int y = (int)evaluate_expr(root->right->right);  // FIXED from `root->right->left`

        if (mem2d.count(root->left->name)) {
            if (x < 0 || x >= mem2d[root->left->name].size() || 
                y < 0 || y >= mem2d[root->left->name][x].size()) {
                yyerror("2D array index out of bounds");
                exit(1);
            }
            return mem2d[root->left->name][x][y];
        }
        else if (fmem2d.count(root->left->name)) {
            if (x < 0 || x >= fmem2d[root->left->name].size() || 
                y < 0 || y >= fmem2d[root->left->name][x].size()) {
                yyerror("2D array index out of bounds");
                exit(1);
            }
            return fmem2d[root->left->name][x][y];
        }
    }

    // Evaluate left and right subtrees
    float left_eval = evaluate_expr(root->left);
    float right_eval = evaluate_expr(root->right);
    float ans = 0;

    // Perform operation based on root->name[0] (assuming operator is stored as a string)
    switch (root->name[0]) {
        case '+': ans = left_eval + right_eval; break;
        case '-': ans = left_eval - right_eval; break;
        case '*': ans = left_eval * right_eval; break;
        case '/':
            if (right_eval == 0) {
                yyerror("Division by zero");
                exit(1);
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
				if(mem.count(root->right->name)){
					cout << mem[root->right->name].first << '\n';
				}
				else if(fmem.count(root->right->name)){
					cout << fmem[root->right->name].first << '\n';
				}
                
            }
            else if(root->right->token == tokenVal) {
                int eval = (int) evaluate_expr(root->right);
                cout << eval << '\n';
            }
            else if(root->right->token == tokenArr){
                string ind = root->right->left->name;
                int val = (int) evaluate_expr(root->right->right);
                cout << mem[ind].second[val] << '\n'; 
            }
			else if(root->right->token == tokenFloat){
				float eval = evaluate_expr(root->right);
				cout << eval << '\n';
			}

			else if(root->right->token == tokenOp){
				float x = evaluate_expr(root->right);
				cout << x << '\n';
			}

			else if(root->right->token == tokenArr){
				int x = (int) evaluate_expr(root->right);
				if(mem.count(root->left->name)){
					cout << mem[root->left->name].second[x] << '\n';
				}
				else if(fmem.count(root->left->name)){
					cout << fmem[root->left->name].second[x] << '\n';
				}
			}

			else if(root->right->token == token2d){
				int x = (int) evaluate_expr(root->right->right->left);
				int y = (int) evaluate_expr(root->right->right->right);
			
				string name = root->right->left->name;

				if(mem2d.count(name)){
					cout << mem2d[name][x][y] << '\n';
				}
				else if(fmem2d.count(name)){
					cout << fmem2d[name][x][y] << '\n';
				}
			}
			
        } 
        
    }

    else if(root->name == "="){
		// cout << "In assign " << root->left->name << '\n';

        if(root->left->token == tokenVar 
		&& !mem.count(root->left->name)
		&& !fmem.count(root->left->name)){
			yyerror("Undefined variable in assign");
			exit(0);
		} 

		if(root->left->token == tokenArr 
		&& !mem.count(root->left->left->name)
		&& !fmem.count(root->left->left->name)){
			yyerror("Undefined variable in assign");
			exit(0);
		}
		
		if(root->left->token == tokenVar && mem.count(root->left->name)){
			int x = (int) evaluate_expr(root->right);

			if(root->left->token == tokenVar){
				mem[root->left->name].first = x;
				// cout << "MEM: " << root->left->name << '\n';
			}
				
			
			else if(root->left->token == tokenArr) {
				int ind = (int) evaluate_expr(root->left->right);
				string name = root->left->left->name;

				// cout << mem[name].second[0] << '\n';
				mem[name].second[ind] = x;
			}
		}

		else if(root->left->token == tokenVar && fmem.count(root->left->name)){
			// implement float here
			float x = evaluate_expr(root->right);

			// cout << "FMEM: " << root->left->name << '\n';

			if(root->left->token == tokenVar)
				fmem[root->left->name].first = x;
			
			else if(root->left->token == tokenArr) {
				int ind = (int) evaluate_expr(root->left->right);
				string name = root->left->left->name;

				// cout << mem[name].second[0] << '\n';
				fmem[name].second[ind] = x;
			}
		}
		else if(root->left->token == tokenArr){
			int ind = (int) evaluate_expr(root->left->right);
			string name = root->left->left->name;

			if(mem.count(name)){
				int x = (int) evaluate_expr(root->right);
				mem[name].second[ind] = x;	
			}
			else if(fmem.count(name)){
				float x = evaluate_expr(root->right);
				fmem[name].second[ind] = x;
			}

			// cout << mem[name].second[0] << '\n';
			
		}

		else if(root->left->token == token2d){
			int x = (int) evaluate_expr(root->left->right->left);
			int y = (int) evaluate_expr(root->left->right->right);

			string name = root->left->left->name;

			if(mem2d.count(name)){
				int val = evaluate_expr(root->right);

				mem2d[name][x][y] = val; 
			}
			else if(fmem2d.count(name)){
				float val = evaluate_expr(root->right);

				fmem2d[name][x][y] = val;
			}
		}
    }

    else if(root->name == "IF_ELSE") {
        int exp = (int) evaluate_expr(root->left);
		// printf("EXP: %d\n", exp);
		if(exp){
			// curr is the stmt_list
			TreeNode* curr = root->right->left;
			// cout << curr->name << '\n';
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
		int flag = (int) evaluate_expr(cond);
		while(flag){
			// cout << "Hello1\n";
			TreeNode* stlist = root->right->right;

			while(stlist){
				execute_stmt(stlist->left);
				stlist = stlist->right;
			}

			execute_stmt(root->right->left); // execute assign
			cond = root->left->right;
			flag = (int) evaluate_expr(cond);
		}
	}
	else if(root->name == "STMT_LIST"){
		execute_stmt(root->left);
		print_tree(root->left);
		execute_stmt(root->right);
	}
	else if(root->name == "DO_WHILE"){
		int condition = (int) evaluate_expr(root->left);
		
		do
		{
			/* code */
			TreeNode* stlist = root->right;

			while(stlist){
				execute_stmt(stlist->left);
				stlist = stlist->right;
			}

			condition = evaluate_expr(root->left);
		} while (condition);
		
	}
}

void declare_vars(TreeNode* root){
	TreeNode* curr = root;

	while(curr){
		if(curr->left->name == "FLOAT"){
			TreeNode* var = curr->left->right;
			
			while(var){
				if(var->token == tokenVar){
					if(!fmem.count(var->name) && !mem.count(var->name))
						fmem[var->name] = {-1.00, {}};
					else{
						yyerror("Redefined variable var: Float");
						// exit(1);
					}
						
				}
				else if(var->token == tokenArr){
					if(!fmem.count(var->left->name) && !mem.count(var->left->name)){
						fmem[var->left->name].first = 0.00;
						fmem[var->left->name].second = vector<float>(var->right->numValue, -1.00);
					}
					else{
						yyerror("Redefined variable array");
						// exit(1);
					}
				}
				// else if(var->left->token == token2d){
				// 	if(!mem2d.count(var->left->name) && !fmem2d.count(var->left->name)){
				// 		fmem2d[var->left->name] = vector<vector<float>>(var->right->left->numValue, vector<float>(var->right->right->numValue, -1.00));
				// 		// mem[var->left->name].second = vector<int>(var->right->numValue, -1);
				// 	}
				// 	else{
				// 		yyerror("Redefined variable array");
				// 		// exit(1);
				// 	}
				// }

				else if (var->token == token2d) {
					if (!mem2d.count(var->left->name) && !fmem2d.count(var->left->name)) {
						fmem2d[var->left->name] = vector<vector<float>>(
							var->right->left->numValue, 
							vector<float>(var->right->right->numValue, -1.00)
						);
					} else {
						yyerror("Redefined variable array");
					}
				}


				var = var->right;
			}
			
		}

		else if(curr->left->name == "INT"){
			TreeNode* var = curr->left->right;
			
			while(var){
				if(var->token == tokenVar){
					if(!mem.count(var->name) && !fmem.count(var->name))
						mem[var->name] = {-1, {}};
					else{
						cout << mem.count(var->name) << '\n';
						yyerror("Redefined variable var: INT");
						// exit(1);
					}	
						
				}
				else if(var->token == tokenArr){
					if(!mem.count(var->left->name) && !fmem.count(var->left->name)){
						mem[var->left->name].first = 0;
						mem[var->left->name].second = vector<int>(var->right->numValue, -1);
					}
					else{
						yyerror("Redefined variable array");
						// exit(1);
					}
				}
				// else if(var->left->token == token2d){
				// 	if(!mem2d.count(var->left->name) && !fmem2d.count(var->left->name)){
				// 		mem2d[var->left->name] = vector<vector<int>>(var->right->left->numValue, vector<int>(var->right->right->numValue, -1));
				// 		// mem[var->left->name].second = vector<int>(var->right->numValue, -1);
				// 	}
				// 	else{
				// 		yyerror("Redefined variable array");
				// 		// exit(1);
				// 	}
				// }
				
				else if (var->token == token2d) {
					if (!mem2d.count(var->left->name) && !fmem2d.count(var->left->name)) {
						mem2d[var->left->name] = vector<vector<int>>(
							var->right->left->numValue, 
							vector<int>(var->right->right->numValue, -1)
						);
					} else {
						yyerror("Redefined variable array");
					}
				}


				var = var->right;
			}
		}

		

		curr = curr->right;
	}
}
