#include <vector>
using namespace std;

typedef enum { typeCon, typeId, typeOpr } nodeEnum;

struct Nodeptr{
    int type;
    int val;
    vector<Nodeptr*> children;

    Nodeptr(nodeEnum _type, int _val){
        type = _type;
        val = _val;
    }

    Nodeptr(nodeEnum _type, int _val, vector<Nodeptr*> _children){
        type = _type;
        val = _val;
        for(auto _child: _children){
            children.push_back(_child);
        }
    }
};

Nodeptr* insert(Nodeptr* root, vector<Nodeptr*> children);
// Nodeptr* remove(Nodeptr* root, Nodeptr* child);