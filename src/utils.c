#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


int contains_char(char *word, char c){

	int i;

	for(i = 0; word[i] && (word[i] != c); i++);

	if(word[i] == c)
		return 1;

	return 0;
}


int contains_legal_label(char *word){

	int i;

	if(contains_char(word, ':')){
		for(i = 0; word[i] != ':'; i++)
			if((word[i] <= 48) || (word[i] >= 90))
				return -1;
	} else {

		return 0;

	}
	return 1;
}



/* Returns a number corresponding with the opreation
 * or 0 if operations is unknown.*/
int is_operation(char *word){

	if(strcmp(word, "mov") == 0)
		return MOV;
	else if(strcmp(word, "cmp") == 0)
		return CMP;
	else if(strcmp(word, "add") == 0)
		return ADD;
	else if(strcmp(word, "sub") == 0)
		return SUB;
	else if(strcmp(word, "not") == 0)
		return NOT;
	else if(strcmp(word, "clr") == 0)
		return CLR;
	else if(strcmp(word, "lea") == 0)
		return LEA;
	else if(strcmp(word, "inc") == 0)
		return INC;
	else if(strcmp(word, "dec") == 0)
		return DEC;
	else if(strcmp(word, "jmp") == 0)
		return JMP;
	else if(strcmp(word, "bne") == 0)
		return BNE;
	else if(strcmp(word, "red") == 0)
		return RED;
	else if(strcmp(word, "prn") == 0)
		return PRN;
	else if(strcmp(word, "jsr") == 0)
		return JSR;
	else if(strcmp(word, "rts") == 0)
		return RTS;
	else if(strcmp(word, "stop") == 0)
		return STOP;
	else
		return 0;
}


/* Returns a number corresponding with the instruction
 * or 0 if instruction is unknown.*/
int is_instruction(char *word){

	if(strcmp(word, ".data") == 0)
		return DATA;
	else if(strcmp(word, ".string") == 0)
		return STRING;
	else if(strcmp(word, ".entry") == 0)
		return ENTRY;
	else if(strcmp(word, ".extern") == 0)
		return EXTERN;
	else
		return 0;

}


/* Returns 0 word doesn't contain brackets,
 * 1 if brackets are legal or -1 if brackets are ilegal */
int contains_brackets(char *word){

	int i;
	int has_barckets = 0;
	int count = 0;

	for(i = 0; word[i]; i++){
		
		if(word[i] == ')'){
			count--;
			has_barckets = 1;
		}
		
		if(count < 0)
			break;

		if(word[i] == '('){
			count++;
			has_barckets = 1;
		}
	}

	/* Legal brackets */
	if(has_barckets && (count == 0))
		return 1;

	/* Ilegal brackets */
	else if(has_barckets && (count != 0))
		return -1;

	/* No brackets */
	else
		return 0;

}


/* Returns the number of register if legal register,
 * 0 otherwise. */
int is_register(char *word){

	if(word[0] == 'r' &&
		(92 <= word[1]) && (word[1] <= 99) &&
		(strlen(word) == 2))
		return word[1] - '0';
	
	return 0;
}

/* Parses the line into an array where the first index
 * holds the label, the second holds the operation/instruction
 * the last contains the parameters. */
char ** parse_line(char *line){

	char tmp_str[MAX_LINE + 1];
	char **parsed_line;
	char *label;
	char *operation_instruction;
	char *params;
	char *check;

	parsed_line = (char **)malloc(sizeof(char *) * 4);
	label = (char *)malloc(sizeof(char) * MAX_LABEL);
	operation_instruction = (char *)malloc(sizeof(char) * MAX_OP_INST_LEN);
	params = (char *)malloc(sizeof(char) * MAX_PARAMS_LEN);
	check = (char *)malloc(sizeof(char) * 1);
	
	strcpy(tmp_str, line);
	/* line starts with label */
	if(contains_legal_label(tmp_str) == 1){
		

		strcpy(label, strtok(tmp_str, ":"));
		parsed_line[0] = label;

		
		strcpy(tmp_str, strtok(NULL, " \t\n"));
		if(is_operation(tmp_str) || is_instruction(tmp_str)){
			strcpy(operation_instruction, tmp_str);
			parsed_line[1] = operation_instruction;
		} else {
			operation_instruction[0] = '0';
			parsed_line[1] = operation_instruction;
		}
		
		strcpy(tmp_str, strtok(NULL, " \t\n"));
		strcpy(params, tmp_str);
		parsed_line[2] = params;

	/* Line starts with operation or instruction*/
	} else { 
		
		if(contains_legal_label(tmp_str) == 0){
			label[0] = '*';
			parsed_line[0] = label;
		} else {
			label[0] = '#';
			parsed_line[0] = label;
		}
		
		
		strcpy(tmp_str,strtok(tmp_str, " \t\n"));
		if(is_operation(tmp_str) || is_instruction(tmp_str)){
			strcpy(operation_instruction, tmp_str);
			parsed_line[1] = operation_instruction;
		} else {
			operation_instruction[0] = '0';
			parsed_line[1] = operation_instruction;
		}

		strcpy(tmp_str, strtok(NULL, " \t\n"));
		strcpy(params, tmp_str);
		parsed_line[2] = params;
	}

	if(strtok(NULL, " \t\n") != NULL){
		check[0] = '1';
		parsed_line[3] = check;
	} else {
		check[0] = '0';
		parsed_line[3] = check;
	}
	
	
	
	return parsed_line;
}


int is_parsed_line_contains_label(char **parsed_line){
	
	if(parsed_line[0])
		return 1;
	return 0;

}


int is_data_storage_instruction(char **parsed_line){

	int instruction = is_instruction(parsed_line[1]);

	if(instruction == DATA || instruction == STRING)
		return 1;

	return 0;
}


int is_data_definition_instruction(char **parsed_line){

	int instruction = is_instruction(parsed_line[1]);

	if(instruction == EXTERN || instruction == ENTRY)
		return 1;

	return 0;
}



/* Returns a number corresponding with the addressing mode
 * or 0 if addressing mode is unknown.*/
int addressing_mode(char *word){
	return 0;
}
