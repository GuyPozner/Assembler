#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


/* A type for representing the symbol table*/
typedef struct symbol{

	char *name;
	unsigned int adress;
	unsigned int is_external:1;
	unsigned int is_operation:1;
	struct symbol *next;

}symbol;



typedef struct symbolTable{

	struct symbol *head;

}symbolTable;


/* Constructs a new symbol*/
symbol * new_symbol(char *name, unsigned int adress,
					unsigned int is_external, unsigned int is_operation){


	symbol *new = (symbol *)malloc(sizeof(symbol));
	new->name = (char *)malloc(sizeof(char) * MAX_LABEL);

	new->next = NULL;
	strcpy(new->name, name);
	new->adress = adress;
	new->is_external = is_external;
	new->is_operation = is_operation;

	return new;

}

/* Adds a symbol to table */
void add_symbol(symbolTable *symbol_table, char *name, unsigned int adress,
				unsigned int is_external, unsigned int is_operation){

	symbol *new, *tmp;

	/* Find the last line in the table */
	tmp = symbol_table->head;
	while(tmp != NULL)
		tmp = tmp->next;

	/* Concatante the symbol at the end of the table */
	new = new_symbol(name, adress, is_external, is_operation);
	tmp->next = new;

}

symbolTable * new_symbol_table(){
	
	symbolTable *new;
	
	new->head = NULL;
	
	return new;
}


void clear_symbol_table(symbolTable *symbol_table){
	
	symbol *tmp1, *tmp2;
	
	tmp1 = symbol_table->head;
	while(tmp1 != NULL){
		tmp2 = tmp1;
		tmp1 = tmp1->next;
		free(tmp2);
	}
}

unsigned int get_symbol_adress(symbolTable *symbol_table, char *name){
	
	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		if(strcmp(tmp->name, name) == 0)
			return tmp->adress;
	}

	return 0;
}

unsigned int is_symbol_operation(symbolTable *symbol_table, char *name){
	
	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		if(strcmp(tmp->name, name) == 0)
			return tmp->is_operation;
	}
	
	return 2;
}

unsigned int is_symbol_external(symbolTable *symbol_table, char *name){
	
	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		if(strcmp(tmp->name, name) == 0)
			return tmp->is_external;
	}
	return 2;
}

