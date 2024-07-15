# Parser and Interpreter for ipl Language

ipl is a programming language with its own syntax that was created for this project.

This program takes as input programs written in ipl, uses a parser to read the code (for syntax errors) and then an interpreter to execute the commands.

The language supports commands such as while, if, else, break, continue,...

Programms written in ipl can be found in the folder programs.

The program kalmpr.ipl is this [project](https://github.com/JennyKozi/Algorithmic_Projects_in_C/tree/main/K-Almost_Primes) written in ipl.

Use the Makefile to compile, run and clean using the following commands:

## Compile Program
```bash
make
```

## Run Program
Use the interpreter to execute different programs written in ipl language:

```bash
./ipli programs/kalmpr.ipl
```
```bash
./ipli programs/selectsort.ipl
```
```bash
./ipli programs/nqueens.ipl
```
```bash
./ipli programs/matrmult.ipl
```
```bash
./ipl programs/numbers.ipl
```
```bash
./ipl programs/countdivs.ipl
```
```bash
./ipl programs/factorize.ipl
```
```bash
./ipl programs/humble.ipl
```
```bash
./ipl programs/primes.ipl
```

## Clean After Execution
```bash
make clean
```