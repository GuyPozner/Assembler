#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/*	Function: second_pass
 *	-----------------------
 *	Performs a second pass on the assembly code,
 *	writes the .ob, .ent and .ext files if no errors
 *	were detected in the .as input file.
 *
 *	@param[in]	filepath Holds the filepath of the input
 *						 file.
 *
 *	@returns: The count of errors detected in second pass.
 */
int second_pass(char *filepath);

#endif