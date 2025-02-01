#include "hoc.h"
#include "y.tab.h"
#include <math.h>

extern double Log(), Log10(), Exp(), Sqrt(), integer();

static struct{
    char* name;
    double cval;
} consts[] = {
    "PI", 3.141593,
    "E", 2.718281,
    "GAMMA", 0.577215,
    "DEG", 57.295780,
    "PHI", 1.618034,
    0, 0
};

static struct {
    char* name;
    double (*func)();
} builtins[] = {
    "sin", sin,
    "cos", cos,
    "atan", atan,
    "log", Log,
    "log10", Log10,
    "exp", Exp,
    "sqrt", Sqrt,
    "int", integer,
    "abs", fabs,
    0, 0
};

int init(){
    Symbol* s;

    for(int i = 0; consts[i].name; i++)
        install(consts[i].name, VAR, consts[i].cval);
    
    for(int i = 0; builtins[i].name; i++){
        s = install(builtins[i].name, BLTIN, 0.0);
        s->u.ptr = builtins[i].func;
    }
}