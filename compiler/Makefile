all:
	yacc -d src/compiler.y
	lex src/compiler.l
	mkdir -p bin
	g++ -I./include -I./src lex.yy.c src/compiler.cpp src/codeGen.cpp -o bin/compiler  y.tab.c -lm
	
clean:
	rm -rf *.tab.c *.yy.c *.tab.[hc] bin/
	rm -rf ./outputs/

rebuild: clean all
	mkdir -p ./outputs
	bin/compiler ../Assignment4_Test_Cases/test1.sil

build_mips:
	mips-linux-gnu-gcc-10  outputs/mips.s  -o mips 
	qemu-mips -L /usr/mips-linux-gnu/ mips

correct_mips:
	mips-linux-gnu-gcc-10  ../Cprograms_and_MIPS_Assembly_Code/s1.s -o s1
	qemu-mips -L /usr/mips-linux-gnu/ s1
