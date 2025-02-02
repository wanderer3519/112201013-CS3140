all:
	lex sample.l
	yacc -d sample.y
	cc lex.yy.c y.tab.c

clean:
	rm -rf *.yy.c  *.tab.[hc] *.out