my_assembler: src/my_assembler.o src/second_pass.o src/first_pass.o src/utils.o src/symbol_table.o
	gcc -g -ansi -pedantic -Wall src/symbol_table.o src/utils.o src/first_pass.o src/second_pass.o src/my_assembler.o  -o my_assembler

src/my_assembler.o: src/my_assembler.c
	gcc -c -ansi -pedantic -Wall  src/my_assembler.c -o src/my_assembler.o

src/second_pass.o: src/second_pass.c
	gcc -c -ansi -pedantic -Wall src/second_pass.c -o src/second_pass.o

src/first_pass.o: src/first_pass.c
	gcc -c -ansi -pedantic -Wall src/first_pass.c -o src/first_pass.o

src/utils.o: src/utils.c
	gcc -c -ansi -pedantic -Wall  src/utils.c -o src/utils.o

src/symbol_table.o: src/symbol_table.c
	gcc -c -ansi -pedantic -Wall src/symbol_table.c -o src/symbol_table.o