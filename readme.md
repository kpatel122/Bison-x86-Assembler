# Bison x86 Assembler
Bison x86 Assembler is a mini assembler based on the x86 instruction set. This project was for my university dissertation on developing  Compilers, Virtual Machines & Graphic Engines.

![Screenshot](https://github.com/kpatel122/Bison-x86-Assembler/Images/BisonASM.jpg)

# Description
The Assembler performs the following operations
* Reducing each instruction to its corresponding opcode.
* Convert all variable and array references into relative stack indices depending on the scope they reside in.
* Take note of each line label and replace it with the instruction index it references.
* Discard all human induced content such as comments or white spaces
* Creating binary bytecode that can run on the Bison Virtual Machine(BVM).


# Documentation
Documentation I wrote for the project (doc files):

* [Bison x86 Assembler](https://github.com/kpatel122/Bison-x86-Assembler/blob/master/Documents/BisonAssembler.doc)

## Languages
C/C++
## Built With
Visual Studio 6

## Author
Kunal Patel

## License
This project is released under the [MIT License](https://opensource.org/licenses/MIT) 

