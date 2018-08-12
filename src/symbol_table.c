#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"


int is_legal_symbol_name(char *word){

	int i;

	for(i = 0; word[i]; i++)
		if((word[i] <= '0') || (word[i] >= 'z'))
			return 0;
	
	return 1;
}

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

/* Adds a symbol to table, returns 1 if symbol
 * was added, 0 otherwise. */
int add_symbol(symbolTable *symbol_table, char *name, unsigned int adress,
				unsigned int is_external, unsigned int is_operation){

	symbol *new, *last;
	
	/* Trying to add an ilegal symbol */
	if(!is_legal_symbol_name(name))
		return 0;

	/* Find the last line in the table */
	last = symbol_table->head;
	if(last == NULL){
		new = new_symbol(name, adress, is_external, is_operation);
		symbol_table->head = new;
		return 1;
	}

	if(strcmp(last->name, name) == 0){
			return 0;
	}

	while(last->next != NULL){
		last = last->next;
		if(strcmp(last->name, name) == 0)
			return 0;
	}
	
	/* Concatante the symbol at the end of the table */
	new = new_symbol(name, adress, is_external, is_operation);
	last->next = new;

	return 1;
}

symbolTable * new_symbol_table(){
	
	symbolTable *new;
	
	new = (symbolTable *)malloc(sizeof(symbolTable));
	new->head = NULL;
	
	return new;
}


void free_symbol_table(symbolTable *symbol_table){
	
	symbol *tmp1, *tmp2;
	
	tmp1 = symbol_table->head;
	while(tmp1 != NULL){
		tmp2 = tmp1;
		tmp1 = tmp1->next;
		free(tmp2->name);
		free(tmp2);
	}
	free(symbol_table);
}

unsigned int get_symbol_adress(symbolTable *symbol_table, char *name){
	
	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		if(strcmp(tmp->name, name) == 0)
			return tmp->adress;
		tmp = tmp->next;
	}

	return -1;
}

unsigned int is_symbol_operation(symbolTable *symbol_table, char *name){
	
	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		if(strcmp(tmp->name, name) == 0)
			return tmp->is_operation;
		tmp = tmp->next;
	}
	
	return -1;
}

unsigned int is_symbol_external(symbolTable *symbol_table, char *name){
	
	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		if(strcmp(tmp->name, name) == 0)
			return tmp->is_external;
		tmp = tmp->next;
	}
	return -1;
}

void update_adresses(symbolTable *symbol_table, int IC){

	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		if((!tmp->is_operation) && (!tmp->is_external))
			tmp->adress = tmp->adress + IC + MEM_ADRESS;
		if((tmp->is_operation))
			tmp->adress = tmp->adress +  MEM_ADRESS;
		tmp = tmp->next;
	}
}

void print_symbol_table(symbolTable *symbol_table){

	symbol *tmp = symbol_table->head;
	while(tmp != NULL){
		printf("name: %s, adress: %d, is_operation: %d, is_external: %d\n", tmp->name, tmp->adress, tmp->is_operation, tmp->is_external);
		tmp = tmp->next;
	}
}
