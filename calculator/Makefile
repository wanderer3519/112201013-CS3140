all:
	lex calculator.l &&\
	yacc -d calculator.y &&\
	cc lex.yy.c y.tab.c -ll -w &&\
	./a.out

clean:
	rm -rf *.yy.c *.tab.c *.tab.h *.out