#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifndef MAX_LABEL
#define MAX_LABEL 31
#endif

#ifndef MEM_ADRESS
#define MEM_ADRESS 100
#endif


/* The symbol struct,
 * a node in a linked list.*/
typedef struct symbol{

	char *name;
	unsigned int adress;
	unsigned int is_external:1;
	unsigned int is_operation:1;
	struct symbol *next;

}symbol;


/*
	The symbolTable, implemented as a linked list,
	to hold the symbols, their addresses and definitions.
*/
typedef struct symbolTable{

	symbol *head;

}symbolTable;



/*	Function: new_symbol_table
 *	--------------------------
 *	Initializes a new symbolTable.
 *
 *	@returns: empty symbolTable. 
 *	
 */
symbolTable * new_symbol_table();


/*	Function: add_symbole
 *	--------------------------
 *	adds a new symbol to the symbol table.
 *
 *	@param[out]	symbol_table The symbol table to add
 *							 the new symbol to.
 *	@param[in]	name 		 The name of the new symbol.
 *	@param[in]	adress 		 The adress of the new symbol.
 *	@param[in]	is_external  Is the new symbol external.
 *	@param[in]	is_operation Is the new symbol an operation.
 *
 *	@returns: 1 if symbol was added, 0 otherwise. 
 *	
 */
int add_symbol(symbolTable *symbol_table, char *name,
 unsigned int adress, unsigned int is_external, unsigned int is_operation);


/*	Function: free_symbol_table
 *	--------------------------
 *
 *	@param[int]	symbol_table The symbol_table
 *							 to be free from memory.
 */
void free_symbol_table(symbolTable *);


/*	Function: get_symbol_adress
 *	--------------------------
 * 	gets the symbol address by label.
 *
 *	@param[int]	symbol_table The symbol_table to search for
 *							 symbol named label in.
 *	@param[in]	label		 The name of the symbol to
 *							 check if is an operation.
 *
 *	@returns: -1 if label is not found, 1 if an operation
 *			  and 0 otherwise.
 *			  
 */
unsigned int get_symbol_adress(symbolTable *symbol_table,
 char *label);


/*	Function: is_symbol_operation
 *	--------------------------
 * 	Check if symbol with name "label" is an operation.
 *
 *	@param[int]	symbol_table The symbol_table to search for
 *							 symbol named label in.
 *	@param[in]	label		 The name of the symbol to
 *							 check if is an operation.
 *
 *	@returns: -1 if label is not found, 1 if an operation
 *			  and 0 otherwise.
 *			  
 */
unsigned int is_symbol_operation(symbolTable *, char *);


/*	Function: is_symbol_external
 *	--------------------------
 * 	Check if symbol with name "label" is external.
 *
 *	@param[int]	symbol_table The symbol_table to search for
 *							 symbol named label in.
 *	@param[in]	label		 The name of the symbol to
 *							 check if is external.
 *
 *	@returns: -1 if label is not found, 1 if external and
 *			  0 otherwise.
 */
unsigned int is_symbol_external(symbolTable *symbol_table
	, char *label);


/*	Function: update_adresses
 *	--------------------------
 *	Function to update the address of symbols 
 *	according to the length of code section and
 *	the MEM_ADDRESS(starting position of the machine code).
 *
 *	@param[out]	symbol_table The symbol
 *				             table to update its addresses.
 *	@param[in]	IC 			 The length of the code section.
 */
void update_adresses(symbolTable *symbol_table, int IC);


/*	Function: print_symbol_table
 *	----------------------------
 *	Function to check the symbol table
 *  created at first pass.
 *
 *	@param[in]	symbol_table The symbol table to
 *							 be printed.
 */
void print_symbol_table(symbolTable *symbol_table);


#endif