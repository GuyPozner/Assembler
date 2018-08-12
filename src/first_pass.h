#ifndef FIRST_PASS_H
#define FIRST_PASS_H

/*	Function: first_pass
 *	-----------------------
 *	Performs a first pass on the assembly code,
 *	Saves and assigns adresses to all the symbols,
 *	Checks code validity, and count the number of 
 *	code and data lines.
 *
 *	@param[in]	filepath Holds the filepath of the input
 *						 file.
 *	@param[out]	symbol_table Hold the symbol table to
 *							 be written in the first pass.
 *
 *	@returns: The count of errors detected in first pass.
 */
int * first_pass(char *filepath, symbolTable *symbol_table);

#endif