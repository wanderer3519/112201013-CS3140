all: calculator.l calculator.y
	yacc -d calculator.y   # Generates y.tab.c and y.tab.h
	lex calculator.l       # Generates lex.yy.c
	g++ lex.yy.c y.tab.c -o calculator

clean:
	rm -rf *.yy.c *.tab.c *.tab.h calculator

gen-file: calculator
	./calculator > output.txt

YFLAGS = -d
OBJS = hoc.o init.o math.o symbol.o

hoc_all: hoc.y
	yacc $(YFLAGS) hoc.y

hoc3: $(OBJS)
	cc $(OBJS) -lm -o hoc3

hoc.o: hoc.h

init.o symbol.o: hoc.h y.tab.h

pr:
	@pr hoc.y hoc.h init.c math.c symbol.c Makefile

clean-hoc:
	rm -f $(OBJS) y.tab.[ch]