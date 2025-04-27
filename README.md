# 112201013-CS3140
# Compiler Design Course (CS3140)

- **Name**: Bhogaraju Shanmukha Sri Krishna
- **Roll Number**: 112201013

## Assignment 4 Instructions

### To Run:

1. Go to compiler directory
```bash
cd compiler/
```

2. Build the executable
```bash
make
```

3. Compile an input file `a.sil`
```bash
./bin/compiler a.sil
```
Output will be found in directory `compiler/`

4. Compile `a.s` to mips executable
```bash
mips-linux-gnu-gcc-10 a.s -o a.o
```

5. Run the object file in qemu
```bash
qemu-mips -L /usr/mips-linux-gnu/ a.o
```

### Implementation Details:
- Compiler executable is located at `compiler/bin/compiler` (after building using make)
- Mips assembly code is generated at `compiler/` location after compiling with `compiler/bin/compiler`

### Screenshots:
- **AST**: 
    <img src="./static/AST.jpeg">

- **Symbol table**: 
    <img src="./static/symtab.jpeg">

- **Mips assembly file**: 
    <img src="./static/mips_file.jpeg">
