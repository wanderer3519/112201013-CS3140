all:
	yacc -d src/compiler.y
	lex src/compiler.l
	mkdir -p bin
	g++ -I./include lex.yy.c -o bin/compiler  y.tab.c -lm
	
clean:
	rm -rf *.tab.c *.yy.c *.tab.[hc] bin/


