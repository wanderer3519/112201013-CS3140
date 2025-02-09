#ifndef TREE_HPP
#define TREE_HPP

#include <cstring>
#include <unordered_map>

using namespace std;

enum NodeType { tokenOp, tokenKey, tokenVar, tokenVal };

struct TreeNode {
    // union{
        TreeNode* left;
        // Symbol* sym_left;
    // };
    

    TreeNode* right;
    NodeType token;
    
    union {
        char* name;     // For operators and keywords
        int numValue;   // For numbers
    };

    // Constructor for operator or keyword nodes or variable
    TreeNode(const char* nodeName, NodeType _type, TreeNode* _left = nullptr, TreeNode* _right = nullptr){
        if (_type == tokenOp || _type == tokenKey) {
            name = new char[strlen(nodeName) + 1];  // Allocate memory for name
            strncpy(name, nodeName, strlen(nodeName) + 1);
        }
        else if(_type == tokenVar){
            name = new char[strlen(nodeName)];  // Allocate memory for name
            strncpy(name, nodeName, strlen(nodeName) - 1);
            name[strlen(nodeName) - 1] = '\0';
        }

        left = _left;
        right = _right;
        token = _type;
    }

    // Constructor for number nodes
    TreeNode(int _value, NodeType _type, TreeNode* _left = nullptr, TreeNode* _right = nullptr) {
        left = _left;
        right = _right;
        numValue = _value;
        token = _type;
    }

};


#endif