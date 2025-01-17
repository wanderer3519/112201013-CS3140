all: 
	lex calculator.l &&\
	yacc -d calculator.y &&\
	cc lex.yy.c y.tab.c -o calc -w -lm

clean:
	rm -rf *.yy.c *.tab.c *.tab.h calc

hoc:
	yacc hoc_all.y &&\
	cc -c yy.tab.c -o hoc1

clean-hoc:
	rm -rf *.tab.c hoc1