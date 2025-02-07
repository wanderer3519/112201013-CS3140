#ifndef TREE_HPP
#define TREE_HPP

#include <cstring>
using namespace std;

enum NodeType { tokenOp, tokenKey, tokenVar, tokenNum };

struct Symbol {
    char* name;
    int type;  // Data type (e.g., int, float, etc.)
    union {
        int intValue;
        float floatValue;
    } value;
};

struct VarClass {
    Symbol* left;  // Symbol table entry
    VarClass* right;  // Next variable in the list
    char* name;  // Variable name

    VarClass(Symbol* sym) : left(sym), right(nullptr) {}
    VarClass(Symbol* sym, VarClass* _right) : left(sym), right(_right) {}
};

struct TreeNode {
    struct TreeNode* left;
    struct TreeNode* right;
    NodeType type;
    
    union {
        char* name;     // For operators and keywords
        int numValue;   // For numbers
        struct VarClass* var;  // For variables linking to the symbol table
    } data;

    // Constructor for operator or keyword nodes
    TreeNode(NodeType type, const char* nodeName, TreeNode* left = nullptr, TreeNode* right = nullptr)
        : left(left), right(right), type(type) {
        if (type == tokenOp || type == tokenKey) {
            data.name = new char[strlen(nodeName) + 1];  // Allocate memory for name
            strcpy(data.name, nodeName);
        }
    }

    // Constructor for number nodes
    TreeNode(int value, TreeNode* left = nullptr, TreeNode* right = nullptr)
        : left(left), right(right), type(tokenNum) {
        data.numValue = value;
    }

    // Constructor for variable nodes
    TreeNode(VarClass* varPtr, TreeNode* left = nullptr, TreeNode* right = nullptr)
        : left(left), right(right), type(tokenVar) {
        data.var = varPtr;
    }
};


#endif