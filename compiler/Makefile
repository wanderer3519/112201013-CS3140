all:
	yacc -d src/compiler.y
	lex src/compiler.l
	mkdir -p bin
	g++ -I./include -I./src lex.yy.c src/compiler.cpp -o bin/compiler  y.tab.c -lm
	
clean:
	rm -rf *.tab.c *.yy.c *.tab.[hc] bin/

rebuild: clean all
	bin/compiler ../assignment-3-testcases/gpt.sil > tree.txt