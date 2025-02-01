#include <vector>
using namespace std;

typedef enum { typeCon, typeId, typeOpr } nodeEnum;

struct Nodeptr{
    int tokenType;
    vector<Nodeptr*> children;

    Nodeptr(int tokenVal){
        tokenType = tokenVal;
    }
};

Nodeptr* insert(Nodeptr* root, vector<Nodeptr*> children);
// Nodeptr* remove(Nodeptr* root, Nodeptr* child);