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

int contains_char(char *, char);

int contains_legal_label(char *);

int is_legal_label(char *);

int is_operation(char *);

int is_two_operands_operation(char *);

int is_one_operand_operation(char *);

int is_no_operand_operation(char *);

int is_instruction(char *);

int contains_brackets(char *);

int is_register(char *);

char ** parse_line(char *);

int is_parsed_line_contains_label(char **);

int is_data_storage_instruction(char *);

int is_data_definition_instruction(char *);

int addressing_mode(char *);

int is_legal_addressing_modes(char *, char *);

int compute_memory_for_data(char *);

int compute_memory_for_string(char *);

int compute_memory_for_code(char *);

int operation_to_code_words(symbolTable *, char **, char *, char *, int, char **, int *);

int string_to_code_words(symbolTable *, char **, char *, char *, int);

int data_to_code_words(symbolTable *, char **, char *, char *, int);

char *write_entrie_line(symbolTable *, char *);


int write_ob_file(char *filepath, char **code_arr, char **data_arr, int DC, int IC);

int write_entries_file(char *filepath, char **entries_arr, int entries_count);

int write_externals_file(char *filepath, char **externals_arr, int external_calls_count);

void free_parsed_line(char **parsed_line);

#endif