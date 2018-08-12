my_assembler: my_assembler.o second_pass.o first_pass.o utils.o symbol_table.o
	gcc -g -ansi -pedantic -Wall symbol_table.o utils.o first_pass.o second_pass.o my_assembler.o  -o my_assembler

my_assembler.o: my_assembler.c
	gcc -c -ansi -pedantic -Wall  my_assembler.c -o my_assembler.o

second_pass.o: second_pass.c
	gcc -c -ansi -pedantic -Wall second_pass.c -o second_pass.o

first_pass.o: first_pass.c
	gcc -c -ansi -pedantic -Wall first_pass.c -o first_pass.o

utils.o: utils.c
	gcc -c -ansi -pedantic -Wall  utils.c -o utils.o

symbol_table.o: symbol_table.c
	gcc -c -ansi -pedantic -Wall symbol_table.c -o symbol_table.o