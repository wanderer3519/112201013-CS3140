all: calculator.l calculator.y
	yacc -d calculator.y   # Generates y.tab.c and y.tab.h
	lex calculator.l       # Generates lex.yy.c
	g++ lex.yy.c y.tab.c -o calculator

clean:
	rm -rf *.yy.c *.tab.c *.tab.h calculator

gen-file: calculator
	./calculator > output.txt

hoc: hoc_all.y
	yacc hoc_all.y &&\
	cc -c yy.tab.c -o hoc1

clean-hoc:
	rm -rf *.tab.c hoc1