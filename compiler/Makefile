all:
	yacc -d src/compiler.y
	lex src/compiler.l
	mkdir -p bin
	g++ -I./include -I./src lex.yy.c src/compiler.cpp src/codeGen.cpp -o bin/compiler  y.tab.c -lm
	
clean:
	rm -rf *.tab.c *.yy.c *.tab.[hc] bin/
	rm -rf ../outputs/*.txt
	rm -rf ./outputs/*.txt

rebuild: clean all
	mkdir -p ./outputs
	bin/compiler ../assignment-3-testcases/my-test.sil