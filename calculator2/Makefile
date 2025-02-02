all:
	yacc -d calc3.y
	lex calc3.l
	cc lex.yy.c y.tab.c inter.c -o calculator

clean:
	rm -rf *.yy.c *.tab.[ch] calculator