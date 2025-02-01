// symbol table entry
typedef struct _symbol{
    char* name;
    short type;
    union{
        double val;
        double (*ptr)();
    }u;
    struct _symbol* next;
}Symbol;

Symbol* install(char* s, int t, double d);
Symbol* lookup(char* s);