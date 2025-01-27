typedef enum { typeCon, typeId, typeOpr } nodeEnum;

// constants
typedef struct _conNodeType{
    int value;
}conNodeType;

// identifiers
typedef struct _idNodeType{
    int i;
}idNodeType;

// operators
typedef struct _oprNodeType{
    int oper;
    int nops;
    struct _nodeType** op;
}oprNodeType;

typedef struct _nodeType{
    nodeEnum type;

    union {
        conNodeType con;
        idNodeType id;
        oprNodeType opr;
    };
}nodeType;

extern int sym[26];