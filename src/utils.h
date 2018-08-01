#ifndef UTILS_H
#define UTILS_H

#ifndef MAX_REG
#define MAX_REG 8
#endif

#ifndef MAX_LINE
#define MAX_LINE 200
#endif

#ifndef MAX_LABEL
#define MAX_LABEL 31
#endif

#ifndef MAX_OP_INST_LEN
#define MAX_OP_INST_LEN 8
#endif

#ifndef MAX_PARAMS_LEN
#define MAX_PARAMS_LEN 162
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
	IMMEDIATE = 1,
	DIRECT,
	JMP_PARAMS,
	REG_DIRECT
};

int contains_char(char *, char);

int contains_legal_label(char *);

int is_label(char *);

int is_operation(char *);

int is_instruction(char *);

int contains_brackets(char *);

int is_register(char *);

char ** parse_line(char *);

int is_parsed_line_contains_label(char **);

int is_data_storage_instruction(char **);

int is_data_definition_instruction(char **);

int addressing_mode(char *);

#endif