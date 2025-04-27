#include "tree.hpp"
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

// extern string file_name;
ofstream outputFileMips("./outputs/mips.s");

#define cout outputFileMips

extern void yyerror(const char *s);
extern unordered_map<string, pair<int, vector<int>>> mem;

int label_count = 0;

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
	.cprestore	16)";
}

void generate_end()
{
	cout << R"(
	move $2, $0
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
	.file	1 ")"
		 << fname
		 << R"("
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module fp=32
	.module nooddspreg
	.abicalls
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
				else if (var->token == tokenArr)
				{
					cout << "\t.globl\t" + var->left->left->name << '\n';
					if (!ct)
					{
						cout << "\t.section\t\t.bss,\"aw\",@nobits\n";
					}

					int arraySize = 4 * var->left->right->numValue;
					cout << "\t.align\t2\n";
					cout << "\t.type\t" + var->left->left->name + ", @object\n";
					cout << "\t.size\t" + var->left->left->name + ", " + to_string(arraySize) + "\n";
					cout << var->left->left->name << ":\n";
					cout << "\t.space\t" + to_string(arraySize) + "\n";
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
	.align	2
$LC1:
	.ascii	"%d\n\000")";

	generate_main();
}

void generate_expr(TreeNode *root)
{
	// Code for a + b + c
	/* la $2, a
	lw $3, 0($2)       // Load value of a into $3
	la $2, b
	lw $2, 0($2)       // Load value of b into $2
	addu $3, $3, $2    // Add a and b, store result in $3
	la $2, c
	lw $2, 0($2)       // Load value of c into $2
	addu $3, $3, $2    // Add result with c, store in $3 */
	if (!root)
		return;

	// Handle unary minus
	if ((root->name == "-") && ((root->left && !root->right) || (!root->left && root->right)))
	{
		TreeNode *child = root->left ? root->left : root->right;
		generate_expr(child);
		cout << "\tsubu $2, $zero, $2" << endl;
		return;
	}

	if (root->left && root->right && root->token == tokenOp)
	{
		generate_expr(root->left);
		cout << "\taddiu $sp, $sp, -4" << endl;
		cout << "\tsw $2, 0($sp)" << endl;

		generate_expr(root->right);
		cout << "\tlw $3, 0($sp)" << endl;
		cout << "\taddiu $sp, $sp, 4" << endl;

		if (root->name == "+")
			cout << "\taddu $2, $3, $2" << endl;
		else if (root->name == "-")
			cout << "\tsubu $2, $3, $2" << endl;
		else if (root->name == "*")
			cout << "\tmul $2, $3, $2" << endl;

		// throws floating point error: should correct later
		else if (root->name == "/")
		{
			cout << "\tdiv $3, $2" << endl;
			cout << "\tmflo $2" << endl;
		}
		else if (root->name == "%")
		{
			cout << "\tdiv $3, $2" << endl;
			cout << "\tmfhi $2" << endl;
		}
		else if (root->name == "&&")
			cout << "\tand $2, $3, $2" << endl;
		else if (root->name == "||")
			cout << "\tor $2, $3, $2" << endl;
		else if (root->name == "==")
			cout << "\tseq $2, $3, $2" << endl;
		else if (root->name == "!=")
			cout << "\tsne $2, $3, $2" << endl;
		else if (root->name == "<")
			cout << "\tslt $2, $3, $2" << endl;
		else if (root->name == "<=")
			cout << "\tsle $2, $3, $2" << endl;
		else if (root->name == ">")
			cout << "\tsgt $2, $3, $2" << endl;
		else if (root->name == ">=")
			cout << "\tsge $2, $3, $2" << endl;
	}

	else if (root->token == tokenVal)
	{
		cout << "\tli $2, " << root->numValue << endl; // Load immediate value into $2
	}
	else if (root->token == tokenVar && mem.count(root->name))
	{
		cout << "\tla $10, " + root->name << endl; // Load address of variable into $10
		cout << "\tlw $2, 0($10)" << endl;		   // Load value from address into $2
	}
	else if (root->token == tokenArr && mem.count(root->left->name))
	{
		// cout << "\tla $10, " + root->left->name << endl;						 // Load address of array into $10
		// cout << "\tli $11, " + to_string(root->right->numValue) << endl; // Load index into $11
		// cout << "\tsll $11, $11, 2" << endl;							 // Multiply index by 4 (word size)
		// cout << "\tadd $10, $10, $11" << endl;							 // Add index to base address
		// cout << "\tlw $2, 0($10)" << endl;
										 
		// Load value from address into $2
		cout << "\t# Compute index for array access: array[i]" << endl;
		generate_expr(root->right); // Evaluate the index expression into $2
		cout << "\tsll $2, $2, 2" << endl; // Multiply index by 4 (word size)

		// Load the base address of the array
		cout << "\tla $10, " << root->left->name << endl; // Load base address of the array into $10

		// Compute the effective address and load the value
		cout << "\tadd $10, $10, $2" << endl; // Add the index offset to the base address
		cout << "\tlw $2, 0($10)" << endl;   // Load the value from the computed address into $2
	}
	else
	{
		printf("%s\n", root->name.c_str());
		yyerror("Invalid expression or undeclared variable");
	}
}

void generate_for_stmt(TreeNode *root)
{
    int current_label = label_count++; // Generate a unique label for this loop
    cout << "# MIPS code for FOR_STMT" << endl;

    // Initialization
    print_code_2(root->left->left); // Initialization statement

    // Start label
    cout << "FOR_START_" << current_label << ":" << endl;

    // Condition
    generate_expr(root->left->right);    // Condition expression
    cout << "\tbeqz $2, FOR_END_" << current_label << endl; // Exit loop if condition is false

    // Loop body
    print_code_2(root->right->right); // Loop body

    // Update statement
    print_code_2(root->right->left); // Update statement
    cout << "\tj FOR_START_" << current_label << endl; // Jump back to start

    // End label
    cout << "FOR_END_" << current_label << ":" << endl;
}

void generate_if_else(TreeNode *root)
{
    int current_label = label_count++;
    cout << "# MIPS code for IF_ELSE" << endl;

    // Condition
    generate_expr(root->left); // Evaluate condition
    cout << "\tbeqz $2, ELSE_LABEL_" << current_label << endl; // Unique label

    // IF block
    print_code_2(root->right->left); // IF block
    cout << "\tj END_IF_" << current_label << endl; // Unique label

    // ELSE block
    cout << "ELSE_LABEL_" << current_label << ":" << endl;
    print_code_2(root->right->right); // ELSE block

    cout << "END_IF_" << current_label << ":" << endl;
}


void generate_assignment(TreeNode *root)
{
	cout << "\t# MIPS code for ASSIGNMENT" << endl;

	if (!root || !root->left || !root->right)
	{
		yyerror("Invalid assignment statement");
		return;
	}

	generate_expr(root->right); // Evaluate the right-hand side into $2

	// Case: variable assignment (e.g., a = ...)
	if (root->left->token == tokenVar && mem.count(root->left->name))
	{
		cout << "\tla $10, " << root->left->name << endl; // Load address of variable
		cout << "\tsw $2, 0($10)" << endl;				  // Store result in variable
	}
	// Case: array assignment (e.g., a[2] = ...)
	else if (root->left->token == tokenArr && mem.count(root->left->left->name))
	{
		// Generate code for array index (assumes index is in right child of left node)
		TreeNode *indexNode = root->left->right;
		if (!indexNode)
		{
			yyerror("Array index missing");
			return;
		}

		// Save the value to be stored
		cout << "\tmove $12, $2" << endl; // Temporarily save RHS value in $12

		generate_expr(indexNode);						  // Result in $2 (index)
		cout << "\tsll $2, $2, 2" << endl;				  // Multiply index by 4 (word size)
		cout << "\tla $10, " << root->left->left->name << endl; // Load base address of array
		cout << "\tadd $10, $10, $2" << endl;			  // Compute effective address
		cout << "\tsw $12, 0($10)" << endl;				  // Store RHS value at calculated address
	}
	else
	{
		yyerror("Invalid lvalue in assignment or undeclared variable/array");
	}
}

void generate_printf(TreeNode *var)
{
	if (!var)
		return;

	if (var->token == tokenVar)
	{
		cout << "\n\tla $4, $LC1\n";
		cout << "\tla $8, " + var->name << endl; // Load address of variable
		cout << "\tlw $5, 0($8)" << endl;		 // Load value into $5 for printf argument
		cout << "\tjal printf" << endl;			 // Read integer into $v0

		// cout << "sw $v0, " << var->numValue << "($sp)" << endl; // Store value in memory
	}
	else if (var->token == tokenArr)
	{
		cout << "\n\tla $4, $LC1\n";
		// Generate code for array index
		generate_expr(var->right);						// Compute array index in $2
		cout << "\tsll $8, $2, 2" << endl;				// Multiply index by 4 (word size)
		cout << "\tla $9, " << var->left->name << endl; // Load base address of array
		cout << "\taddu $9, $9, $8" << endl;			// Add offset to base address
		cout << "\tlw $5, 0($9)" << endl;				// Load array element value into $5 for printf
		cout << "\tjal printf" << endl;
		// Read integer into $v0

		// cout << "sw $v0, " << var->numValue << "($sp)" << endl; // Store value in memory
	}
	else if (var->token == tokenOp)
	{
		cout << "\n\tla $4, $LC1\n";
		generate_expr(var);
		cout << "\tmove $5, $2" << endl; // Move result from $2 to $5 for printf argument
		cout << "\tjal printf" << endl;	 // Read integer into $v0
	}
	else if (var->token == tokenVal)
	{
		cout << "\n\tla $4, $LC1\n";
		cout << "\tli $5, " << var->numValue << endl; // Load immediate value into $5 for printf
		cout << "\tjal printf" << endl;				  // Read integer into $v0
	}
	else
	{
		yyerror("Invalid variable or undeclared variable");
	}
}

void generate_write(TreeNode *root)
{
	cout << "# MIPS code for WRITE" << endl;

	if (!root)
	{
		return;
	}

	cout << "# MIPS code for READ" << endl;
	TreeNode *st_list = root->right;

	while (st_list)
	{
		TreeNode *var = st_list->left;
		if (!var)
			var = st_list;

		// if(var->token != tokenVal && var->token != tokenOp)
		generate_printf(var);

		st_list = st_list->right;
	}
	// generate_printf(st_list);
}

void generate_scanf(TreeNode *var)
{
	if (!var)
		return;

	// printf("%s\n", var->name.c_str());

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
		// Generate code for array index
		generate_expr(var->right);						// Compute array index in $2
		cout << "\tsll $8, $2, 2" << endl;				// Multiply index by 4 (word size)
		cout << "\tla $9, " << var->left->name << endl; // Load base address of array
		cout << "\taddu $5, $9, $8" << endl;			// Add offset to base address and store in $5 for scanf
		cout << "\tjal __isoc99_scanf" << endl;
		// Read integer into memory location
	}
}

void generate_read(TreeNode *root)
{
	cout << "# MIPS code for READ" << endl;

	TreeNode *st_list = root->right;

	while (st_list)
	{
		TreeNode *var = st_list->left;
		if (!var)
		{
			// Handle case where this is the last node (it contains the actual variable)
			var = st_list;
		}

		generate_scanf(var); // Generate code for reading the variable

		// Move to the next variable in the list
		st_list = st_list->right;
	}
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