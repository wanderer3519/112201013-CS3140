#ifndef TREE_HPP
#define TREE_HPP

#include <cstring>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

enum NodeType { tokenOp, tokenKey, tokenVar, tokenVal };


// Symbol structure to store variable info
// struct Symbol {
//     int id;
//     std::string var_name;
//     std::string var_type;
//     std::shared_ptr<void> value; // Can be used to hold different types (generic token storage)

//     Symbol(int id, std::string var_name, std::string var_type, std::shared_ptr<void> value)
//         : id(id), var_name(var_name), var_type(var_type), value(value) {}
// };

// SymbolTable class that manages symbols by both id and var_name
// class SymbolTable {
// public:
//     // Insert a symbol into the table
//     void insert(int id, const std::string& var_name, int var_type, std::shared_ptr<void> value) {
//         Symbol newSymbol(id, var_name, var_type, value);
//         symbolById[id] = newSymbol;
//         symbolByName[var_name] = newSymbol;
//     }

//     // Lookup a symbol by id
//     std::shared_ptr<Symbol> lookupById(int id) {
//         if (symbolById.find(id) != symbolById.end()) {
//             return std::make_shared<Symbol>(symbolById[id]);
//         }
//         return nullptr;
//     }

//     // Lookup a symbol by name
//     std::shared_ptr<Symbol> lookupByName(const std::string& var_name) {
//         if (symbolByName.find(var_name) != symbolByName.end()) {
//             return std::make_shared<Symbol>(symbolByName[var_name]);
//         }
//         return nullptr;
//     }

//     // Print all symbols
//     void print() {
//         std::cout << "Symbol Table:\n";
//         for (const auto& entry : symbolById) {
//             const Symbol& sym = entry.second;
//             std::cout << "ID: " << sym.id << ", Name: " << sym.var_name
//                       << ", Type: " << sym.var_type << ", Value: ";
//             if (sym.var_type == "int") {
//                 std::cout << *std::static_pointer_cast<int>(sym.value) << std::endl;
//             } else if (sym.var_type == "float") {
//                 std::cout << *std::static_pointer_cast<float>(sym.value) << std::endl;
//             } else {
//                 std::cout << "Unknown token\n";
//             }
//         }
//     }

// private:
//     std::unordered_map<int, Symbol> symbolById;       // Map for id lookup
//     std::unordered_map<std::string, Symbol> symbolByName; // Map for name lookup
// };

// unordered_map<string, int> sym_tab;


// struct VarClass {
//     Symbol* left;  // Symbol table entry
//     VarClass* right;  // Next variable in the list
//     char* name;  // Variable name

//     VarClass(Symbol* sym) : left(sym), right(nullptr) {}
//     VarClass(Symbol* sym, VarClass* _right) : left(sym), right(_right) {}
// };

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
        // VarClass* var;  // For variables linking to the symbol table
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

        // printf("NAME: %s\n", name);
        // name = "Nothing";
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

    // Constructor for variable nodes
    // TreeNode(VarClass* varPtr, TreeNode* left = nullptr, TreeNode* right = nullptr)
    //     : left(left), right(right), token(tokenVar) {
    //     data.var = varPtr;
    // }
};


#endif