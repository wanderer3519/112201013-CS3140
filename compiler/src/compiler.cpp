#include "tree.hpp"
#include <bits/stdc++.h>
#include <iomanip>


using namespace std;

extern void yyerror(const char *s);
extern unordered_map<string, pair<int, vector<int>>> mem;
extern unordered_set<string> boolVars;

/*	Helper function to print one level of a binary tree */
void print_level(TreeNode *root, int t_level, int p_level, int height)
{
	/*
		Inputs:
			1.	Top level node t
			2.	Height of the top level node (t) in the whole binary tree
			3.	Level of the binary tree to be printed
			4.	Height of the whole binary tree
	*/
	if (t_level > p_level)
	{
		fprintf(stderr, "Error in print_level\n");
		return;
	}
	if (t_level < p_level)
	{
		if (root != NULL)
			print_level(root->left, t_level + 1, p_level, height);
		else
			print_level(NULL, t_level + 1, p_level, height);
		if (root != NULL)
			print_level(root->right, t_level + 1, p_level, height);
		else
			print_level(NULL, t_level + 1, p_level, height);
		return;
	}
	// t_level == p_level
	int padding = 1; // for alignment
	for (int i = p_level; i < height - 1; i++)
		padding = padding * 2 + 1;
	// To prefix some space and a line
	for (int i = 0; i < (padding + 1) / 2; i++)
		printf("  ");
	for (int i = (padding + 1) / 2; i < padding; i++)
		printf("__");
	if (root != NULL)
	{
		switch (root->token)
		{
		// case tokenVar:
		// // printf("%s", root->name);
		// cout << root->name;
		// break;
		case tokenOp:
			// printf("%s", root->name);
			if (root->name[0] == '+')
			{
				printf("PLUS");
			}
			else if (root->name[0] == '-')
			{
				printf("MINUS");
			}
			else if (root->name[0] == '*')
			{
				printf("MULT");
			}
			else if (root->name[0] == '/')
			{
				printf("DIV");
			}
			else if (root->name[0] == '=')
			{
				printf("ASSIGN");
			}
			else
			{
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
	for (int i = (padding + 1) / 2; i < padding; i++)
		printf("__");
	for (int i = 0; i < (padding + 1) / 2; i++)
		printf("  ");
	printf("  ");
	return;
}

int height(TreeNode *root)
{
	if (!root)
		return 0;
	int left = height(root->left);
	int right = height(root->right);
	return 1 + max(left, right);
}
/*	Helper function to print the binary tree in a tree-like way. */
void print_tree(TreeNode *t)
{
	int h = height(t);
	for (int l = 0; l < h; l++)
	{
		print_level(t, 0, l, h);
		printf("\n");
	}
	return;
}

void print_proper(TreeNode *root, string prefix, bool isLeft)
{
	if (!root)
		return;

	// Print current node
	cout << prefix;
	cout << (isLeft ? "├── " : "└── ");

	// Print node label with full operator handling
	if (root->token == tokenOp)
	{
		if (root->name == "+")
			cout << "PLUS";
		else if (root->name == "-")
			cout << "MINUS";
		else if (root->name == "*")
			cout << "MULT";
		else if (root->name == "/")
			cout << "DIV";
		else if (root->name == "%")
			cout << "MOD";
		else if (root->name == "=")
			cout << "ASSIGN";
		else if (root->name == "==")
			cout << "EQUAL";
		else if (root->name == "!=")
			cout << "NOT_EQUAL";
		else if (root->name == "<")
			cout << "LESS_THAN";
		else if (root->name == "<=")
			cout << "LESS_EQUAL";
		else if (root->name == ">")
			cout << "GREATER_THAN";
		else if (root->name == ">=")
			cout << "GREATER_EQUAL";
		else if (root->name == "&&")
			cout << "LOGICAL_AND";
		else if (root->name == "||")
			cout << "LOGICAL_OR";
		else if (root->name == "!")
			cout << "LOGICAL_NOT";
		else
			cout << root->name; // Fallback case for unknown ops
	}
	else if (root->token == tokenVal)
	{
		cout << root->numValue;
	}
	else
	{
		cout << root->name;
	}
	cout << endl;

	// Construct the prefix for child nodes
	string newPrefix = prefix + (isLeft ? "│   " : "    ");

	// Print left and right children
	if (root->left || root->right)
	{
		print_proper(root->left, newPrefix, true);
		print_proper(root->right, newPrefix, false);
	}
}

int evaluate_expr(TreeNode *root)
{
	if (!root)
		return 0;
	if (root->token == tokenVal)
		return root->numValue;
	else if (root->token == tokenVar && !mem.count(root->name))
	{
		// printf("VAR: %s|\n", root->name);
		// char* str;
		// sprintf(str, "Undefined Variable %s", root->name);
		yyerror("Undefined Variable in Eval expr");
		exit(0);
	}
	else if (root->token == tokenVar)
	{
		return mem[root->name].first;
	}

	else if (root->token == tokenArr)
	{
		string name = root->left->name;

		int ind = evaluate_expr(root->right);

		return mem[name].second[ind];
	}

	int left_eval = evaluate_expr(root->left);
	int right_eval = evaluate_expr(root->right);
	int ans = 0;

	if (root->name == "+")
	{
		ans = left_eval + right_eval;
	}
	else if (root->name == "-")
	{
		ans = left_eval - right_eval;
	}
	else if (root->name == "*")
	{
		ans = left_eval * right_eval;
	}
	else if (root->name == "/")
	{
		if (right_eval == 0)
		{
			yyerror("Division by zero");
			exit(0);
		}
		ans = left_eval / right_eval;
	}
	else if (root->name == "%")
	{
		ans = left_eval % right_eval;
	}
	else if (root->name == ">")
	{
		ans = (left_eval > right_eval);
	}
	else if (root->name == "<")
	{
		ans = (left_eval < right_eval);
	}
	else if (root->name == "==")
	{
		ans = (left_eval == right_eval);
	}
	else if (root->name == "!=")
	{
		ans = (left_eval != right_eval);
	}
	else if (root->name == ">=")
	{
		ans = (left_eval >= right_eval);
	}
	else if (root->name == "<=")
	{
		ans = (left_eval <= right_eval);
	}
	else
	{
		cout << root->name << '\n';
		yyerror("Unknown operator");
		exit(0);
	}

	return ans;
}

bool breakFlag = false; // Global flag for break handling

void execute_stmt(TreeNode *root)
{
	if (!root)
		return;

	if (breakFlag)
	{
		// breakFlag = false;
		return;
	}

	// cout << "DEBUG: " << root->name << '\n';

	/* Write every statement as a function then use them */

	if (root->name == "WRITE")
	{
		// $3 = root->right
		// string printing
		if (root->right->name == "STRING")
			cout << root->right->left->name << '\n';

		// expr printing
		else
		{

			if (root->right->token == tokenVar)
			{
				cout << mem[root->right->name].first << '\n';
			}
			else if (root->right->token == tokenVal)
			{
				int eval = evaluate_expr(root->right);
				cout << eval << '\n';
			}
			else if (root->right->token == tokenArr)
			{
				string ind = root->right->left->name;
				int val = evaluate_expr(root->right->right);
				cout << mem[ind].second[val] << '\n';
			}

			else if (root->right->token == tokenStr)
			{
				cout << root->right->name << '\n';
			}
		}
	}

	else if (root->name == "=")
	{
		if (root->left->token == tokenVar && !mem.count(root->left->name))
		{
			// print_symbol_table(mem);
			yyerror("Undefined variable var in assign");
			exit(0);
		}

		if (root->left->token == tokenArr && !mem.count(root->left->left->name))
		{
			// print_symbol_table(mem);
			yyerror("Undefined variable arr in assign");
			exit(0);
		}

		string name = "";
		if(root->left->token == tokenVal){
			name = root->left->name;
		}
		else if(root->left->token == tokenArr){
			name = root->left->left->name;
		}

		// string name = (root->left->token == tokenVal) ? root->left->name : root->left->left->name;
		
		if(boolVars.count(name)){
			TreeNode* rexp = root->right;
			int rval = evaluate_expr(rexp);

			if(rval != 0 && rval != 1){
				yyerror("Type mismatch: Assignment of type 'integer' to type 'boolean'");
				exit(0);
			}
		}
		
		int x = evaluate_expr(root->right);

		if (root->left->token == tokenVar)
			mem[root->left->name].first = x;

		else if (root->left->token == tokenArr)
		{
			int ind = evaluate_expr(root->left->right);
			string name = root->left->left->name;

			// cout << mem[name].second[0] << '\n';
			mem[name].second[ind] = x;
		}
	}

	else if (root->name == "IF_ELSE")
	{
		int exp = evaluate_expr(root->left);
		// printf("EXP: %d\n", exp);
		if (exp)
		{
			// curr is the stmt_list
			TreeNode *curr = root->right->left;
			// cout << curr->name << '\n';
			while (curr)
			{
				execute_stmt(curr->left);
				curr = curr->right;
			}
		}

		else
		{
			// curr is the stmt_list
			TreeNode *curr = root->right->right;

			while (curr)
			{
				execute_stmt(curr->left);
				curr = curr->right;
			}
		}
	}
	else if (root->name == "FOR_STMT")
	{
		execute_stmt(root->left->left); // execute assign

		TreeNode *cond = root->left->right;
		int flag = evaluate_expr(cond);
		while (flag)
		{
			// cout << "Hello1\n";

			TreeNode *stlist = root->right->right;
			TreeNode *stHead = stlist;

			while (stlist)
			{
				execute_stmt(stlist->left);
				if (breakFlag)
					break;
				stlist = stlist->right;
			}

			if (breakFlag)
			{					   // Stop execution if break is encountered
				breakFlag = false; // Reset for future loops
				break;
			}

			execute_stmt(root->right->left);

			cond = root->left->right;
			flag = evaluate_expr(cond);
			stlist = stHead;
		}
	}
	else if (root->name == "STMT_LIST")
	{
		execute_stmt(root->left);
		// print_proper(root->left, "", true);
		execute_stmt(root->right);
	}
	else if (root->name == "BREAK")
	{
		breakFlag = true;
	}
}

void declare_vars(TreeNode *root)
{
	TreeNode *curr = root;

	while (curr)
	{
		if (curr->left->name == "INT")
		{
			TreeNode *var = curr->left->right;

			while (var)
			{
				if (var->token == tokenVar)
				{
					if (!mem.count(var->name))
						mem[var->name] = {-1, {}};
					else
					{
						cout << mem.count(var->name) << '\n';
						yyerror("Redefined variable var: INT");
						// exit(1);
					}
				}
				else if (var->token == tokenArr)
				{
					string name = var->left->left->name;
					int size = var->left->right->numValue;

					if (!mem.count(name))
					{
						mem[name].first = 0;
						mem[name].second = vector<int>(size, -1);
					}
					else
					{
						yyerror("Redefined variable array");
						// exit(1);
					}
				}

				var = var->right;
			}
		}

		else if (curr->left->name == "BOOL")
		{
			TreeNode *var = curr->left->right;

			while (var)
			{
				if (var->token == tokenVar)
				{
					if (!mem.count(var->name)){
						mem[var->name] = {0, {}};
						boolVars.insert(var->name);
					}
						
					else
					{
						cout << mem.count(var->name) << '\n';
						yyerror("Redefined variable var: BOOL");
						// exit(1);
					}
				}
				else if (var->token == tokenArr)
				{
					string name = var->left->left->name;
					int size = var->left->right->numValue;

					if (!mem.count(name))
					{
						mem[name].first = 0;
						mem[name].second = vector<int>(size, -1);
						boolVars.insert(name);
					}
					else
					{
						yyerror("Redefined variable array");
						// exit(1);
					}
				}

				var = var->right;
			}
		}

		curr = curr->right;
	}
}
void print_symbol_table(const unordered_map<string, pair<int, vector<int>>> &mem)
{
	cout << "Symbol Table:\n";
	cout << "---------------------------------------------------------------------\n";
	cout << "| Variable  | Type    | Structure | Size | Value                    |\n";
	cout << "---------------------------------------------------------------------\n";

	for (const auto &entry : mem)
	{
		const string &varName = entry.first;
		int value = entry.second.first;
		const vector<int> &arr = entry.second.second;
		
		string structure = arr.empty() ? "scalar" : "array";
		string typeDisplay = boolVars.count(varName) ? "boolean" : "integer";
		int size = arr.empty() ? 1 : arr.size();

		cout << "| " << setw(9) << left << varName << " | "
			 << setw(7) << left << typeDisplay << " | "
			 << setw(9) << left << structure << " | "
			 << setw(4) << left << size << " | ";

		if (arr.empty())
		{
			cout << setw(24) << left << value;
		}
		else
		{
			string arrValues = "[";
			for (size_t i = 0; i < min(size_t(3), arr.size()); i++)
			{
				arrValues += to_string(arr[i]);
				if (i != min(size_t(2), arr.size() - 1))
					arrValues += ",";
			}
			if (arr.size() > 3)
				arrValues += "...";
			arrValues += "]";
			cout << setw(24) << left << arrValues;
		}
		cout << " |\n";
	}
	cout << "---------------------------------------------------------------------\n";
}
