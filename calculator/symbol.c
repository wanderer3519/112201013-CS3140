#include "hoc.h"
#include "y.tab.h"
#include <string.h>

static Symbol* symlist = 0;

Symbol* lookup(char* s){
    Symbol* sp = symlist;
    while(sp){
        if(strcmp(sp->name, s) == 0)
            return sp;
        sp = sp->next;
    }
    return 0;
}

char* emalloc(unsigned n){
    char* p = malloc(n);
    if(!p)
        execerror("out of memory", (char*) 0);

    return p;
}

Symbol* install(char* s, int t, double d){
    Symbol* sp = emalloc(sizeof(Symbol));

    sp->name = emalloc(strlen(s) + 1);
    strcpy(sp->name, s);
    sp->type = t;
    sp->u.val = d;
    sp->next = symlist;
    symlist = sp;
    
    return sp;
}