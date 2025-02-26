#ifndef TREE_HPP
#define TREE_HPP

#include <bits/stdc++.h>

using namespace std;

enum NodeType { tokenOp, tokenKey, tokenVar, tokenVal, tokenArr, tokenStr };

struct TreeNode {
    TreeNode* left;
    TreeNode* right;
    NodeType token;
    
    
    string name;     // For operators and keywords
    int numValue;   // For numbers
    

    // Constructor for operator or keyword nodes or variable
    TreeNode(const char* nodeName, NodeType _type, TreeNode* _left = nullptr, TreeNode* _right = nullptr){
        // if (_type != tokenVal) {
        //     name = new char[strlen(nodeName) + 1];  // Allocate memory for name
        //     strncpy(name, nodeName, strlen(nodeName));
        //     name[strlen(nodeName)] = '\0';
        // }
        name = string(nodeName);

        left = _left;
        right = _right;
        token = _type;
    }

    // Constructor for number nodes
    TreeNode(int _value, NodeType _type, TreeNode* _left = nullptr, TreeNode* _right = nullptr) {
        if(_type == tokenVal){
            left = _left;
            right = _right;
            numValue = _value;
            token = _type;
        }
        
    }


};


#endif