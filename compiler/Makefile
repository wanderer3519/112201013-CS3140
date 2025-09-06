all:
	yacc -d src/compiler.y
	lex src/compiler.l
	mkdir -p bin
	g++ -I./include -I./src lex.yy.c src/compiler.cpp src/codeGen.cpp -o bin/compiler  y.tab.c -lm
	
clean:
	rm -rf *.tab.c *.yy.c *.tab.[hc] bin/
	rm -rf ./outputs/
	rm -rf *.out
	rm -rf *.s

rebuild: clean all
	mkdir -p ./outputs
	bin/compiler ../tests/Assignment4_Test_Cases/sample4.sil

build_mips:
	mips-linux-gnu-gcc-10  outputs/mips.s
	qemu-mips -L /usr/mips-linux-gnu/ a.out

correct_mips:
	mips-linux-gnu-gcc-10  ../tests/Cprograms_and_MIPS_Assembly_Code/s1.s -o s1
	qemu-mips -L /usr/mips-linux-gnu/ s1
