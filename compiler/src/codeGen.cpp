#include "tree.hpp"
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

ofstream outputFileMips("./outputs/mips.s");

#define cout outputFileMips

extern void yyerror(const char *s);
extern unordered_map<string, pair<int, vector<int>>> mem;

void generate_main()
{
	cout << 
R"(
	.text
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,32,$31		# vars= 0, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	reorder
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$fp,24($sp)
	move	$fp,$sp
	.cprestore	16
	)";
}

void generate_end(){
	cout << R"(
	move	$sp,$fp
	lw	$31,28($sp)
	lw	$fp,24($sp)
	addiu	$sp,$sp,32
	j	$31
	.end	main
	.size	main, .-main
	.ident	"GCC: (Ubuntu 10.3.0-1ubuntu1) 10.3.0"
	.section	.note.GNU-stack,"",@progbits
	)";
}

/**
 * Corrected MIPS code generation for FOR_STMT and other constructs.
 */

void init_code(string filename)
{
	string fname = filename.substr(filename.find_last_of('/') + 1);
	// string fname = filename;
	cout << R"(
	.file	1 ")"  << fname 
		<< R"("
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module fp=32
	.module nooddspreg
	.text)";
}

void generate_vars(TreeNode *root)
{

	cout << "# MIPS code for variable declarations" << endl;
	TreeNode *curr = root;

	while (curr)
	{
		if (curr->left->name == "INT")
		{
			TreeNode *var = curr->left->right;
			int ct = 0;
			while (var)
			{
				if (var->token == tokenVar)
				{
					if (!mem.count(var->name))
					{
						mem[var->name] = {-1, {}};

						cout << "\t.globl\t" + var->name << '\n';
						if (!ct)
						{
							cout << "\t.section\t\t.bss,\"aw\",@nobits\n";
						}

						cout << "\t.align\t2\n";
						cout << "\t.type\t" + var->name + ", @object\n";
						cout << "\t.size\t" + var->name + ", 4\n";
						cout << var->name << ":\n";
						cout << "\t.space\t4\n";
					}

					else
					{
						cout << mem.count(var->name) << '\n';
						yyerror("Redefined variable var: INT");
						// exit(1);
					}
				}
				else if (var->token == tokenArr)
				{
					cout << "\t.globl\t" + var->name << '\n';
					if (!ct)
					{
						cout << "\t.section\t\t.bss,\"aw\",@nobits\n";
					}

					cout << "\t.align\t2\n";
					cout << "\t.type\t" + var->name + ", @object\n";
					cout << "\t.size\t" + var->name + ", 4\n";
					cout << var->name << ":\n";
					cout << "\t.space\t" + to_string(4 * var->left->right->numValue) + "\n";
				}

				var = var->right;

				if (!ct)
					ct++;
			}
		}

		else if (curr->left->name == "BOOL")
		{
		}

		curr = curr->right;
	}

	cout << R"(
	.rdata
	.align	2
$LC0:
	.ascii	"%d\000"
	)";

	generate_main();
}

void generate_expr(TreeNode *root)
{
	if (!root)
		return;
	if (root->token == tokenVal)
	{
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

void generate_for_stmt(TreeNode *root)
{
	cout << "# MIPS code for FOR_STMT" << endl;

	// Initialization
	print_code_2(root->left->left); // Initialization statement

	cout << "FOR_START:" << endl;

	// Condition
	generate_expr(root->left->right);	 // Condition expression
	cout << "beqz $t0, FOR_END" << endl; // Exit loop if condition is false

	// Loop body
	print_code_2(root->right->right); // Loop body

	// Update statement
	print_code_2(root->right->left); // Update statement
	cout << "j FOR_START" << endl;	 // Jump back to start

	cout << "FOR_END:" << endl;
}

void generate_if_else(TreeNode *root)
{
	cout << "# MIPS code for IF_ELSE" << endl;

	// Condition
	generate_expr(root->left);				// Evaluate condition
	cout << "beqz $t0, ELSE_LABEL" << endl; // Branch to ELSE if condition is false

	// IF block
	print_code_2(root->right->left); // IF block
	cout << "j END_IF" << endl;		 // Skip ELSE block

	// ELSE block
	cout << "ELSE_LABEL:" << endl;
	print_code_2(root->right->right); // ELSE block

	cout << "END_IF:" << endl;
}

void generate_assignment(TreeNode *root)
{
	cout << "# MIPS code for ASSIGNMENT" << endl;
	generate_expr(root->right);									   // Evaluate right-hand side
	cout << "sw $t0, " << root->left->numValue << "($sp)" << endl; // Store value in memory
}

void generate_write(TreeNode *root)
{
	cout << "# MIPS code for WRITE" << endl;

	if (!root)
	{
		return;
	}

	// print_code_2(root->left); // Evaluate expression
	generate_expr(root->right); // Evaluate expression

	cout << "li $v0, 1" << endl;	 // Load syscall for print integer
	cout << "move $a0, $t0" << endl; // Move result to $a0
	cout << "syscall" << endl;
}

void generate_scanf(TreeNode *var)
{
	if(!var)
		return;

	if (var->token == tokenVar)
	{
		cout << "\n\tla $4, $LC0\n";
		cout << "\tla $5," + var->name << endl; // Load syscall for read integer
		cout << "\tjal __isoc99_scanf" << endl; // Read integer into $v0
		// cout << "sw $v0, " << var->numValue << "($sp)" << endl; // Store value in memory
	}
	else if (var->token == tokenArr)
	{
		cout << "\n\tla $4, $LC0\n";
		cout << "\tli $5," + var->left->name + to_string(var->right->numValue * 4) << endl; // Load syscall for read integer
		cout << "\tjal __isoc99_scanf" << endl;											  // Read integer into $v0
		// cout << "sw $v0, " << var->numValue << "($sp)" << endl; // Store value in memory
	}
}

void generate_read(TreeNode *root)
{
	/*
	la	$7,c
	la	$6,b
	la	$5,a
	la	$4,$LC0
	jal	__isoc99_scanf

	la	$7,array+8
	la	$6,array+4
	la	$5,array
	la	$4,$LC0
	jal	__isoc99_scanf

	*/


	cout << "# MIPS code for READ" << endl;
	TreeNode *st_list = root->right;

	while (st_list)
	{
		TreeNode *var = st_list->left;
		if (!var)
			break;
		

		generate_scanf(var);

		st_list = st_list->right;
	}

	// printf("%s\n", st_list->name.c_str());
	generate_scanf(st_list);
}

void generate_break()
{
	cout << "# MIPS code for BREAK" << endl;
	cout << "j FOR_END" << endl; // Jump to end of loop
}

void print_code_2(TreeNode *root)
{
	if (!root)
		return;

	if (root->name == "WRITE")
	{
		generate_write(root);
	}
	else if (root->name == "READ")
	{
		generate_read(root);
	}
	else if (root->name == "=")
	{
		generate_assignment(root);
	}
	else if (root->name == "IF_ELSE")
	{
		generate_if_else(root);
	}
	else if (root->name == "FOR_STMT")
	{
		generate_for_stmt(root);
	}
	else if (root->name == "STMT_LIST")
	{
		print_code_2(root->left);
		print_code_2(root->right);
	}
	else if (root->name == "BREAK")
	{
		generate_break();
	}
	else
	{
		// Handle other cases or default behavior
		cout << "# Unknown node type: " << root->name << endl;
	}
}

void print_code(TreeNode *root)
{
	print_code_2(root);
	generate_end();
}