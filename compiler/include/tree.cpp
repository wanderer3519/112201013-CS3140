#include <bits/stdc++.h>
#include "tree.hpp"
using namespace std;

Nodeptr* insert(Nodeptr* root, vector<Nodeptr*> children){
    for(auto child: children){
        root->children.push_back(child);
    }
    return root;
}