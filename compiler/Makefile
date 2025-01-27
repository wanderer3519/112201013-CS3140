all:
	lex src/compiler.l
	gcc src/lex.yy.c 
	./a.out 
clean:
	rm -rf *.tab.c *.yy.c *.tab.h *.out