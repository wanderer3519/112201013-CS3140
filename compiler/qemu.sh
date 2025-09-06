#!/bin/bash
#install dependendies 
sudo apt install binutils-mips-linux-gnu
sudo apt install gcc-10-mips-linux-gnu
sudo apt install qemu-user-static
sudo apt  install qemu-user
#done with installing packages
#let us run a sample c code.
mips-linux-gnu-gcc-10 file.c -S -o file.s
# you have to generate assembly code (.s file) from the AST for the input program.
# then it will be compiled and run with below commands
mips-linux-gnu-gcc-10  file.s -o file.out
qemu-mips -L /usr/mips-linux-gnu/ file.out

