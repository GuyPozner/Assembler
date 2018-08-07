#ifndef UTILS_H
#define UTILS_H

#ifndef MAX_REG
#define MAX_REG 8
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
	IMMEDIATE = 0,
	DIRECT,
	JMP_PARAMS,
	REG_DIRECT
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

int operation_to_code_words(char **, char *, char *, int);

#endif