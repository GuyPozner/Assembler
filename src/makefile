first_pass: first_pass.o utils.o symbol_table.o
	gcc -g -ansi -pedantic -Wall symbol_table.o utils.o first_pass.o -o first_pass

first_pass.o: first_pass.c
	gcc -c -ansi -pedantic -Wall first_pass.c -o first_pass.o

utils.o: utils.c
	gcc -c -ansi -pedantic -Wall utils.c -o utils.o

symbol_table.o: symbol_table.c
	gcc -c -ansi -pedantic -Wall symbol_table.c -o symbol_table.o