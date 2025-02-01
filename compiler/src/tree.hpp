#ifndef TREE_H
#define TREE_H

#include <vector>
using namespace std;

struct Node{
    char* name;
    int type;
    int value;

    Node* left;
    Node* right;
    Node* next;

    Node(char* _name, int _type, Node* _left = nullptr, Node* _right = nullptr){
        name = _name;
        type = _type;
        left = _left;
        right = _right;
    }
};

struct Symbol{
    char* name;
    int type;
    Symbol* next;

    Symbol(char* _name, int _type, Symbol* _next = nullptr): name(_name), type(_type), next(_next) {}
};

struct Func{
    char* name;
    int returnType;
    Symbol* params;
    Func* next;

    Func(char* _name, int _retType, Symbol* _params = nullptr, Func* _next = nullptr): 
        name(_name), returnType(_retType), params(_params), next(_next) {};
};

#endif