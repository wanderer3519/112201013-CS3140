all:
	yacc -d src/compiler.y
	lex src/compiler.l
	g++ -I./include lex.yy.c  y.tab.c -lm
	
clean:
	rm -rf *.tab.c *.yy.c *.tab.[hc] *.out


