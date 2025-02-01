#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

typedef enum { tokenOp, tokenNum, tokenVar }nodeEnum;

#include <iostream>
#include <cstdlib>

struct TreeNode {
    nodeEnum type;
    int varIndex;
    int value;
    TreeNode* left;
    TreeNode* right;

    // Constructors
    // TreeNode(double val) : op(0), value(val), varIndex(-1), left(nullptr), right(nullptr) {}
    // TreeNode(int varIdx) : op(0), value(0.0), varIndex(varIdx), left(nullptr), right(nullptr) {}
    // TreeNode(char oper, TreeNode* l, TreeNode* r) : op(oper), value(0.0), varIndex(-1), left(l), right(r) {}
    TreeNode(nodeEnum _type, int _value){
        type = _type;
        value = _value;
    }
    
    TreeNode(nodeEnum _type, int _value, TreeNode* _left, TreeNode* _right){
        type = _type;
        value = _value;
        left = _left;
        right = _right;
    }
};

// Function to print AST


#endif
