#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifndef MAX_LABEL
#define MAX_LABEL 31
#endif

#ifndef MEM_ADRESS
#define MEM_ADRESS 100
#endif

typedef struct symbol{

	
	char *name;
	unsigned int adress;
	unsigned int is_external:1;
	unsigned int is_operation:1;
	struct symbol *next;

}symbol;



typedef struct symbolTable{

	symbol *head;

}symbolTable;


/* Constructs a new symbol*/
symbol * new_symbol(char *, unsigned int, unsigned int, unsigned int);

int add_symbol(symbolTable *, char *, unsigned int, unsigned int, unsigned int);

symbolTable * new_symbol_table();

void free_symbol_table(symbolTable *);

unsigned int get_symbol_adress(symbolTable *, char *);

unsigned int is_symbol_operation(symbolTable *, char *);

unsigned int is_symbol_external(symbolTable *, char *);

void update_data_adresses(symbolTable *, int);

void print_symbol_table(symbolTable *);


#endif