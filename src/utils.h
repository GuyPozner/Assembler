#ifndef UTILS_H
#define UTILS_H


#ifndef MAX_REG
#define MAX_REG 7
#endif


#ifndef EMPTY_WORD
#define EMPTY_WORD "**************"
#endif


#ifndef LINE_BUF_LEN
#define LINE_BUF_LEN 600
#endif


#ifndef MEM_ADRESS
#define MEM_ADRESS 100
#endif


#ifndef MAX_LINE
#define MAX_LINE 80
#endif


#ifndef MAX_LABEL
#define MAX_LABEL 31
#endif

#ifndef MAX_OP_INST_LEN
#define MAX_OP_INST_LEN 8
#endif


#ifndef MEMORY_WORD_LEN
#define MEMORY_WORD_LEN 14
#endif


#ifndef MAX_PARAMS_LEN
#define MAX_PARAMS_LEN 41
#endif


enum Boolean{
	FALSE = 0,
	TRUE = 1
};


enum FileStatus{
	BAD_FILE = 0,
	GOOD_FILE = 1
};


enum BracketStatus{
	ILEGAL_BRACKETS = -1,
	NO_BRACKETS = 0,
	LEGAL_BRACKETS = 1
};


enum LabelStatus{
	ILEGAL_LABEL = -1,
	NO_LABEL = 0,
	LEGAL_LABEL = 1
};


enum Opration{
	MOV = 1,
	CMP, 
	ADD, 
	SUB, 
	NOT, 
	CLR, 
	LEA, 
	INC, 
	DEC, 
	JMP, 
	BNE, 
	RED, 
	PRN, 
	JSR, 
	RTS, 
	STOP
};


enum Instruction{
	DATA = 1,
	STRING, 
	ENTRY, 
	EXTERN
};


enum AddressingMode{
	NO_PARAM = -2,
	ILEGAL_ADDRESSING_MODE = -1,
	IMMEDIATE = 0,
	DIRECT = 1,
	JMP_PARAMS = 2,
	REG_DIRECT = 3
};


enum Linkage{
	ABS = 0,
	EXT,
	RELOC
};

/*	Function: is_legal_label
 *	------------------------
 *
 *	@param[in]	word The word to test.
 *
 *	@returns: TRUE if word is a legal label, FALSE otherwise.
 */
int is_legal_label(char *word);


/*	Function: is_operation
 *	------------------------
 *
 *	@param[in]	word The word to test.
 *
 *	@returns: The operation number, 0 if not an operation.
 */
int is_operation(char *word);


/*	Function: is_instruction
 *	------------------------
 *
 *	@param[in]	word The word to test.
 *
 *	@returns: The instruction number, 0 if not an instruction.
 */
int is_instruction(char *word);


/*	Function: is_register
 *	------------------------
 *
 *	@param[in]	word The word to test.
 *
 *	@returns: The register number, -1 if not a register.
 */
int is_register(char *word);


/*	Function: parse_line
 *	--------------------
 *
 *	@param[in]	line The assembly line to parse.
 *
 *	@returns: An array of char * containing
 *	the elements of the line. 
 *	parsed_line[0] contains the label.
 *	parsed_line[1] contains the operation or instruction.
 *	parsed_line[2] contains the parameters.
 *	An element which contains * signifies an empty argument,
 *	while an element which contains
 *	# signifies an ilegal argument.
 */
char ** parse_line(char *line);


/*	Function: is_data_storage_instruction
 *	-------------------------------------
 *
 *	@param[in]	word The word to test.
 *
 *	@returns: TRUE if word is a data storage instruction,
 *			  .data or .string, FALSE otherwise.
 */
int is_data_storage_instruction(char *word);


/*	Function: is_data_definition_instruction
 *	----------------------------------------
 *
 *	@param[in]	word The word to test.
 *
 *	@returns: TRUE if word is a data definition instruction,
 *			  .entry or .extern, FALSE otherwise.
 */
int is_data_definition_instruction(char *word);


/*	Function: is_legal_addressing_modes
 *	-----------------------------------
 *
 *	@param[in]	operation The operation to check 
 *						  allowed addressing modes.
 *	@param[in]	params The parameters for the operation
 *					   to check if their addressing mode
 *					   is allowed for operation.
 *
 *	@returns: TRUE if the parmaeters addressing modes
 *			  are allowed for operation, FALSE otherwise.
 */
int is_legal_addressing_modes(char *operation, char *params);


/*	Function: compute_memory_for_data
 *	---------------------------------
 *
 *	@param[in]	params The parameters to compute the
 *					   memory size needed to store integers.
 *
 *	@returns: The memory size need to store the data.
 */
int compute_memory_for_data(char *params);


/*	Function: compute_memory_for_string
 *	-----------------------------------
 *
 *	@param[in]	params The parameters to compute the
 *					   memory size needed to store string.
 *
 *	@returns: The memory size need to store the string.
 */
int compute_memory_for_string(char *);


/*	Function: compute_memory_for_code
 *	---------------------------------
 *
 *	@param[in]	params The parameters to compute the
 *					   memory size needed to store the
 *					   operation.
 *
 *	@returns: The memory size need to store the operation.
 */
int compute_memory_for_code(char *params);


/*	Function: operation_to_code_words
 *	---------------------------------
 *
 *	@param[in]	symbol_table The symbol_table
 *							 holds the addresses
 *							 and definitions of the symbols.
 *	@param[out]	code_arr The "string" array that holds the
 *						 machine code, the function writes
 *						 into it.
 *	@param[in]	operation The operation to be written in
 *						  the first word code.
 *	@param[in]	params    The parameters to take write
 *						  their addressing modes and
 *						  adresses if needed.
 *	@param[in]	IC_ind	  The index in the code array to write
 *						  into.
 *	@param[out]	externals_arr The "string" array of external
 *							  calls position.
 *	@param[out]	external_calls_count The count of calls to
 *									 external variables within
 *									 the code.
 *
 *	@returns: FALSE if a label was searched for
 *			  and not found, FALSE otherwise.
 */
int operation_to_code_words(symbolTable *symbol_talbe,
 char **code_arr, char *operation, char *params, int IC_ind,
  char **externals_arr, int *external_calls_count);


/*	Function: string_to_code_words
 *	------------------------------
 *
 *	@param[in]	symbol_table The symbol_table
 *							 holds the addresses
 *							 and definitions of the symbols.
 *	@param[out]	dara_arr The "string" array that holds the
 *						 data in machine code,
 *						 the function writes into it.
 *	@param[in]	label    The label/symbol that references
 *						 the string.
 *	@param[in]	str      The parameters to write
 *						 their values into the data
 *						 array.
 *	@param[in]	IC	     The number of code_lines,
 *						 code lines that come before
 *                       the data.
 *
 *	@returns: FALSE if label is external,
 *			  TRUE otherwise.
 */
int string_to_code_words(symbolTable *symbol_table,
 char **data_arr, char *label, char *str, int IC);


/*	Function: data_to_code_words
 *	----------------------------
 *
 *	@param[in]	symbol_table The symbol_table
 *							 holds the addresses
 *							 and definitions of the symbols.
 *	@param[out]	dara_arr The "string" array that holds the
 *						 data in machine code,
 *						 the function writes into it.
 *	@param[in]	label    The label/symbol that references
 *						 the string.
 *	@param[in]	data_str The integers to write
 *						 their values into the data
 *						 array.
 *	@param[in]	IC	     The number of code lines,
 *						 that come before the data.
 *
 *	@returns: FALSE if label is external,
 *			  TRUE otherwise.
 */
int data_to_code_words(symbolTable *symbol_table, 
	char **data_arr, char *label, char *data_str, int IC);


/*	Function: write_entrie_line
 *	---------------------------
 *
 *	@param[in]	symbol_table The symbol_table
 *							 holds the addresses
 *							 and definitions of the symbols.
 *	@param[in]	label    The label/symbol that references
 *						 the address in memory to link as
 *						 entry.
 *
 *	@returns: a line formatted for the .ent file.
 */
char *write_entrie_line(symbolTable *symbol_table, char *label);


/*	Function: write_ob_file
 *	-----------------------
 *	writes the ob file.
 *
 *	@param[in]	filepath Holds the filepath of the input
 *						 file.
 *	@param[in]	code_arr Holds the machine code
 *						 for the code section in
 *						 weird base2.
 *	@param[in]	data_arr Holds the machine code
 *						 for the data section in
 *						 weird base2.
 *	@param[in]	IC       The length of the code section.
 *	@param[in]	DC       The length of the data section.
 *
 *	@returns: GOOD_FILE if file was written,
 *			  BAD_FILE otherwise.
 */
int write_ob_file(char *filepath, char **code_arr, char **data_arr, int DC, int IC);


/*	Function: write_entries_file
 *	-----------------------
 *	writes the ent file.
 *
 *	@param[in]	filepath Holds the filepath of the input
 *						 file.
 *	@param[in]	entries_arr Holds the lines formated
 *							for the .ent file.
 *	@param[in]	entries_count The count of entry variables.
 *
 *	@returns: GOOD_FILE if file was written,
 *			  BAD_FILE otherwise.
 */
int write_entries_file(char *filepath, char **entries_arr, int entries_count);


/*	Function: write_externals_file
 *	-----------------------
 *	writes the ext file.
 *
 *	@param[in]	filepath Holds the filepath of the input
 *						 file.
 *	@param[in]	externals_arr Holds the lines formated
 *							for the .ext file.
 *	@param[in]	external_calls_count The number of external
 *									 calls within the code.
 *
 *	@returns: GOOD_FILE if file was written,
 *			  BAD_FILE otherwise.
 */
int write_externals_file(char *filepath, char **externals_arr, int external_calls_count);


/*	Function: free_parsed_line
 *	--------------------------
 *	@param[out]	parsed_line The pointer array to be
 *							free from memory.
 */
void free_parsed_line(char **parsed_line);

#endif