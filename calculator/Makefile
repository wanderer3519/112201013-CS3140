all: calculator.l calculator.y
	lex calculator.l &&\
	yacc -d calculator.y &&\
	cc lex.yy.c y.tab.c -o calculator -w -lm

clean:
	rm -rf *.yy.c *.tab.c *.tab.h calculator

hoc: hoc_all.y
	yacc hoc_all.y &&\
	cc -c yy.tab.c -o hoc1

clean-hoc:
	rm -rf *.tab.c hoc1