all:
	yacc -d calc3.y
	lex calc3.l
	gcc lex.yy.c y.tab.c inter.c -o calculator
	./calculator

clean:
	rm -rf *.yy.c *.tab.[ch] calculator