#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
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


int is_legal_label(char *word){

	int i;

	for(i = 0; word[i]; i++)
		if((word[i] <= 48) || (word[i] >= 90))
			return 0;
	
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
		('0' <= word[1]) && (word[1] <= '7') &&
		(strlen(word) == 2))
		return (word[1] - '0');
	
	return -1;
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
	char *tmp;

	parsed_line = (char **)malloc(sizeof(char *) * 4);
	label = (char *)malloc(sizeof(char) * MAX_LABEL);
	operation_instruction = (char *)malloc(sizeof(char) * MAX_OP_INST_LEN);
	params = (char *)malloc(sizeof(char) * MAX_PARAMS_LEN);
	
	if(line[0] == ';'){
		label[0] = '*';
		operation_instruction[0] = '*';
		params[0] = '*';
	} else {

		strcpy(tmp_str, line);
		/* line starts with label */
		if(contains_legal_label(tmp_str) == 1){
			
			strcpy(label, strtok(tmp_str, ":"));
			if((tmp = strtok(NULL, " \t\n")) != NULL){
				
				strcpy(tmp_str, tmp);
				if(is_operation(tmp_str) || is_instruction(tmp_str))
					strcpy(operation_instruction, tmp_str);
				else 
					operation_instruction[0] = '#';
				
				
				if((tmp = strtok(NULL, "\n")) != NULL)
					strcpy(params, tmp);
				else 
					params[0] = '#';
				
			} else {
				operation_instruction[0] = '#';
				params[0] = '#';
			}
			

		/* Line starts with operation or instruction*/
		} else { 
			
			if(contains_legal_label(tmp_str) == 0)
				label[0] = '*';
			else
				label[0] = '#';
				
			
			if((tmp = strtok(tmp_str, " \t\n")) != NULL){
				
				strcpy(tmp_str, tmp);
				if(is_operation(tmp_str) || is_instruction(tmp_str))
					strcpy(operation_instruction, tmp_str);
				else 
					operation_instruction[0] = '#';
				
				
				if((tmp = strtok(NULL, "\n")) != NULL)
					strcpy(params, tmp);
				else 
					params[0] = '*';
				

			} else {
				
				operation_instruction[0] = '*';
				params[0] = '*';

			}
		}
	}

	
	parsed_line[0] = label;
	parsed_line[1] = operation_instruction;
	parsed_line[2] = params;
	
	return parsed_line;
}


int is_parsed_line_contains_label(char **parsed_line){
	
	if(parsed_line[0])
		return 1;
	return 0;

}


int is_data_storage_instruction(char *word){

	int instruction = is_instruction(word);

	if(instruction == DATA || instruction == STRING)
		return 1;

	return 0;
}


int is_data_definition_instruction(char *word){

	int instruction = is_instruction(word);

	if(instruction == EXTERN || instruction == ENTRY)
		return 1;

	return 0;
}

int is_two_operands_operation(char *operation){
	
	int op_num = is_operation(operation);

	if(op_num == MOV ||
		op_num == CMP ||
		op_num == ADD ||
		op_num == SUB ||
		op_num == LEA)
		return 1;

	return 0;
}

int is_one_operand_operation(char *operation){
	
	int op_num = is_operation(operation);

	if(op_num == NOT ||
		op_num == CLR ||
		op_num == INC ||
		op_num == DEC ||
		op_num == JMP ||
		op_num == BNE ||
		op_num == RED ||
		op_num == PRN ||
		op_num == JSR)
		return 1;
	
	return 0;
}


int is_no_operand_operation(char *operation){
	
	int op_num = is_operation(operation);

	if(op_num == RTS ||
		op_num == STOP)
		return 1;
	
	return 0;
}


int legal_operation_jmp_adressing_with_params(char *operation){
	int op_num = is_operation(operation);

	if(op_num == JMP ||
		op_num == JSR ||
		op_num == BNE)
		return 1;
	
	return 0;
}

int first_left_bracket_ind(char *params){

	int i = 0;
	while(params[i] != '(')
		++i;

	return i;
}

char * substr(char *str, int start, int end){

	char *sub_str;
	sub_str = (char *)malloc(sizeof(char) * ((end - start) + 1));
	

	memcpy(sub_str, str + start, end - start);
	sub_str[(end - start)] = '\0';
	
	return sub_str;

}

char * get_label_from_jmp_params(char *params){
	
	return substr(params, 0, first_left_bracket_ind(params));

}

char * get_params_from_jmp_params(char *params){

	return substr(params, first_left_bracket_ind(params) + 1, 
		strlen(params) - 1);
}



int is_legal_constant(char *cnst){

	int i;

	if(cnst[0] != '#')
		return 0;

	if(cnst[1] == '-' || cnst [1] == '+')
		i = 2;
	else
		i = 1;

	for(;cnst[i];i++)
		if(cnst[i] <= '0' || cnst[i] >= '9')
			return 0;

	return 1;
}

int is_legal_param(char *param){
	
	if(is_legal_label(param) ||
		is_legal_constant(param) ||
		is_register(param))
		return 1;

	return 0;
}

int is_legal_jmp_adressing_with_params_mode(char *params){

	char *label;
	char *in_brackets;
	char *rhs_param, *lhs_param;
	

	label = get_label_from_jmp_params(params);
	if(!is_legal_label(label)){
		free(label);
		return 0;
	}

	in_brackets = substr(params,
						first_left_bracket_ind(params) + 1,
						strlen(params) - 1);
	if(contains_char(in_brackets, ',')){
		
		if(((lhs_param = strtok(in_brackets, ",")) != NULL) &&
			((rhs_param = strtok(NULL, ",")) != NULL)){
			if(is_legal_param(lhs_param) && is_legal_param(rhs_param)){
				free(label);
				free(in_brackets);
				return 1;
			}
		}

	}

	free(label);
	free(in_brackets);
	return 0;

}


/* Returns a number corresponding with the addressing mode
 * or -1 if addressing mode is unknown.*/
int addressing_mode(char *param){

	if(is_legal_constant(param))
		return IMMEDIATE;
	else if(is_legal_label(param))
		return DIRECT;
	else if(is_register(param))
		return REG_DIRECT;
	else if(is_legal_jmp_adressing_with_params_mode(param))
		return JMP_PARAMS;
	else
		return -1;
}

int * params_to_addressing_modes(char *params){

	int *addressing_modes = (int *)malloc(sizeof(int) * 2);
	char *rhs_param, *lhs_param;
	char *tmp_params = (char *)malloc(sizeof(char) * (strlen(params) + 1));
	
	strcpy(tmp_params, params);

	/* If two params */
	if(contains_char(params, ',') && (!(contains_char(params, '(')))){
		if((lhs_param = strtok(tmp_params, " ,\t")) != NULL &&
			(rhs_param = strtok(NULL, " ,\t")) != NULL){
			addressing_modes[0] = addressing_mode(lhs_param);
			addressing_modes[1] = addressing_mode(rhs_param);
			free(tmp_params);
			return addressing_modes;
		} else {
			addressing_modes[0] = -1;
			addressing_modes[1] = -1;
			free(tmp_params);
			return addressing_modes;
		}
	} else {
		addressing_modes[0] = -2;
		addressing_modes[1] = addressing_mode(params);
		free(tmp_params);
		return addressing_modes;
	}
}

int * get_addressing_modes_from_jmp_params(char *params){
	char *jmp_params = substr(params,
			first_left_bracket_ind(params) + 1, (strlen(params) - 1));
	
	return params_to_addressing_modes(jmp_params);
}

int is_legal_addressing_modes(char *operation, char *params){

	int op_num = is_operation(operation);
	int *addressing_modes;

	addressing_modes = (int *)malloc(sizeof(int) * 2);
	addressing_modes = params_to_addressing_modes(params);


	if(op_num == MOV || op_num == ADD || op_num == SUB)
		if((addressing_modes[0] == IMMEDIATE ||
			addressing_modes[0] == DIRECT ||
			addressing_modes[0] == REG_DIRECT) &&
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT)){
			free(addressing_modes);
			return 1;
	}

	if(op_num == CMP)
		if((addressing_modes[0] == IMMEDIATE ||
			addressing_modes[0] == DIRECT ||
			addressing_modes[0] == REG_DIRECT) &&
			(addressing_modes[0] == IMMEDIATE ||
			 addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT)){
			free(addressing_modes);
			return 1;
	}

	if(op_num == NOT || op_num == CLR || op_num == INC ||
	 op_num == DEC || op_num == RED)
		if((addressing_modes[0] == -2) &&
			(addressing_modes[1] == DIRECT ||
			addressing_modes[1] == REG_DIRECT)){
			free(addressing_modes);
			return 1;
		}

	if(op_num == LEA)
		if((addressing_modes[0] == DIRECT) &&
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT)){
			free(addressing_modes);
			return 1;
	}

	if(op_num == JMP || op_num == BNE || op_num == JSR)
		if((addressing_modes[0] == -2) &&
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT ||
			 addressing_modes[1] == JMP_PARAMS)){
			free(addressing_modes);
			return 1;
	}

	if(op_num == PRN)
		if((addressing_modes[0] == -2) && 
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT ||
			 addressing_modes[1] == IMMEDIATE)){
			free(addressing_modes);
			return 1;
		}

	return 0;
	
}


int compute_memory_for_data(char *params){

	char *str_num;
	char *str;
	int memory_size = 0;
	char tmp_params[MAX_PARAMS_LEN];

	strcpy(tmp_params, params);
	if((str_num = strtok(tmp_params," \t,")) == NULL){
		return 0;

	} else {

		strtol(str_num, &str, 10);
		if(str[0])
			return 0;
		else
			++memory_size;
	}

	while((str_num = strtok(NULL, " \t,")) != NULL){
		
		strtol(str_num, &str, 10);
		if(str[0])
			return 0;
		else
			++memory_size;
	}

	return memory_size;
}


int compute_memory_for_string(char *params){

	if(params[0] != '"' ||
		params[strlen(params) - 1] != '"')
		return 0;
	else
		/* An extra space is added for the 0 terminating the string */
		return strlen(params) - 1;
	
}

int compute_memory_for_code(char *params){
	int *addressing_modes, *jmp_addressing_modes;
	int memory_size = 1;

	addressing_modes = (int *)malloc(sizeof(int) * 2);
	addressing_modes = params_to_addressing_modes(params);

	/* Both params are registers*/
	if(addressing_modes[0] == REG_DIRECT &&
		addressing_modes[1] == REG_DIRECT)
		memory_size++;

	/* Jmp addressing with params */
	else if(addressing_modes[0] == -2 && addressing_modes[1] == JMP_PARAMS){
		jmp_addressing_modes = (int *)malloc(sizeof(int) * 2);
		jmp_addressing_modes = get_addressing_modes_from_jmp_params(params);
	
		/* Both params are registers*/
		if(jmp_addressing_modes[0] == REG_DIRECT &&
		   jmp_addressing_modes[1] == REG_DIRECT)
				memory_size += 2;
		else{
			memory_size += 3;
		}
		free(jmp_addressing_modes);

	/* If two params operation */	
	} else if((addressing_modes[0] == DIRECT ||
				addressing_modes[0] == REG_DIRECT ||
				addressing_modes[0] == IMMEDIATE) &&
				(addressing_modes[1] == DIRECT ||
				addressing_modes[1] == REG_DIRECT ||
				addressing_modes[1] == IMMEDIATE))
		memory_size += 2;

	/* If one param */
	else if(addressing_modes[0] == -2)
		memory_size++;


	return memory_size;
}

char * int_to_wierd_base2(int num, int len){
	int i;
	char *base2_str;
	base2_str = (char *)malloc(sizeof(char) * (len + 1));

	for(i = 0; i < len; i++){
		if((1 << i) & num)
			base2_str[(len - 1) - i] = '/';
		else
			base2_str[(len - 1) - i] = '*';		  
	}
	
	/* Terminate string */
	base2_str[len] = '\0';
	return base2_str;
}

char * operation_to_code_word(int param1, int param2, int op_code,
							 int src_addressing, int dest_addressing){

	char *memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));

	memory_word = int_to_wierd_base2(param1, 2);
	memory_word = strcat(memory_word, int_to_wierd_base2(param2, 2));
	memory_word = strcat(memory_word, int_to_wierd_base2(op_code, 4));
	memory_word = strcat(memory_word, int_to_wierd_base2(src_addressing, 2));
	memory_word = strcat(memory_word, int_to_wierd_base2(dest_addressing, 2));
	memory_word = strcat(memory_word, int_to_wierd_base2(ABS, 2));

	return memory_word; 

}


char * label_to_code_word(symbolTable *symbol_table, char *label){

	char *memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));
	int adress;
	int is_external;


	if((adress = get_symbol_adress(symbol_table, label)) == -1)
		return NULL;

	is_external = is_symbol_external(symbol_table, label);
	
	if(is_external){
		
		memory_word = strcat(memory_word, int_to_wierd_base2(0, 12));
		memory_word = strcat(memory_word, int_to_wierd_base2(EXT, 2));

	} else {
		
		memory_word = strcat(memory_word, int_to_wierd_base2(adress, 12));
		memory_word = strcat(memory_word, int_to_wierd_base2(RELOC, 2));
	}

	return memory_word; 

}

char * registers_to_code_word(int src_reg, int dest_reg){

	char *memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));
	
	memory_word = int_to_wierd_base2(src_reg, 6);
	memory_word = strcat(memory_word, int_to_wierd_base2(dest_reg, 6));
	memory_word = strcat(memory_word, int_to_wierd_base2(ABS, 2));

	return memory_word;
}


char * constant_to_code_word(int cnst){

	char *memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));
	
	memory_word = int_to_wierd_base2(cnst, 12);
	memory_word = strcat(memory_word, int_to_wierd_base2(ABS, 2));

	return memory_word;
}

char **parse_params(char *params){

	char *tmp_params = (char *)malloc(sizeof(char) * (strlen(params) + 1));
	char **params_arr = (char **)malloc(sizeof(char *) * 2);
	char *tmp;
	
	params_arr[0] = (char *)malloc(sizeof(char) * (strlen(params) + 1));
	params_arr[1] = (char *)malloc(sizeof(char) * (strlen(params) + 1));


	strcpy(tmp_params, params);
	tmp = strtok(tmp_params, " ,\t");
	strcpy(params_arr[0], tmp);
	if((tmp = strtok(NULL, " ,\t")) != NULL){
		strcpy(params_arr[1], tmp);
	}
	else{

		strcpy(params_arr[1], params_arr[0]);
		strcpy(params_arr[0], "0");

	}

	return params_arr;
}

int constant_str_to_int(char *cnst){

	char *str;
	int retval = strtol(cnst, &str, 10);

	return retval;

}

int param_to_code_word(symbolTable *symbol_table, char **code_arr,
 							char *param, int IC_ind){
	
	char *code_word;

	printf("is_reg: %d\n", is_register(param));
	if(0 <= is_register(param) && is_register(param) <= 7){
		code_arr[IC_ind] = registers_to_code_word(0, is_register(param));
		return 1;
	}
	
	if(is_legal_constant(param)){
		

		code_arr[IC_ind] = constant_to_code_word(constant_str_to_int(param));
		return 1;

	/* Param is label */
	} else {

		code_word = label_to_code_word(symbol_table, param);
		if(code_word == NULL)
			return 0;
		else{
			code_arr[IC_ind] = code_word;
			return 1;
		}
	}
}

int two_params_to_code_words(symbolTable *symbol_table, char **code_arr,
 							char *params, int IC_ind){

	char **params_arr = parse_params(params);

	if((!param_to_code_word(symbol_table, code_arr, params_arr[0], IC_ind)) || 
		(!param_to_code_word(symbol_table, code_arr, params_arr[1], IC_ind + 1)))
		return 0;

	return 1;
}


int operation_with_jmp_addressing_to_code_words(symbolTable *symbol_table, char **code_arr,char *operation,
 							char *params, int IC_ind){

	int src_reg, dest_reg;
	int op_code = is_operation(operation) - 1;
	int *jmp_addressing_modes = get_addressing_modes_from_jmp_params(params);
	int param1 = 0;
	int param2 = 0;
	char *label = get_label_from_jmp_params(params);
	char *jmp_params = get_params_from_jmp_params(params);
	char **jmp_params_arr = parse_params(jmp_params);


	param1 = jmp_addressing_modes[0];
	param2 = jmp_addressing_modes[1];
	
	code_arr[IC_ind] = operation_to_code_word(param1, param2, op_code, 0, JMP_PARAMS);
	code_arr[IC_ind + 1] = label_to_code_word(symbol_table, label);
	

	/* If label wasn't found in symbol table */
	if((code_arr[IC_ind + 1]) == NULL)
		return 0;

	if(param1 == REG_DIRECT && param2 == REG_DIRECT){

		src_reg = is_register(jmp_params_arr[0]);
		dest_reg = is_register(jmp_params_arr[1]);
		code_arr[IC_ind + 2] = registers_to_code_word(src_reg, dest_reg);
		return 1;
	} else {
		if(!(two_params_to_code_words(symbol_table, code_arr, jmp_params, IC_ind + 2)));
			return 0;
	}
	
	return 1;


}



int operation_to_code_words(symbolTable *symbol_table, char **code_arr,char *operation,
 							char *params, int IC_ind){



	int label_found;
	int *addressing_modes;
	int src_reg, dest_reg;
	int op_code = is_operation(operation) - 1;
	char **params_arr = (char **)malloc(sizeof(char) * 2);


	/* If operation has no params */
	if((op_code + 1) == STOP || (op_code + 1) == RTS){
		code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, 0, 0);
		return 1;
	}

	

	addressing_modes = (int *)malloc(sizeof(int) * 2);
	addressing_modes = params_to_addressing_modes(params);
	
	/* Addressing mode jmp with params */
	if(addressing_modes[0] == -2 && addressing_modes[1] == JMP_PARAMS){
		return operation_with_jmp_addressing_to_code_words(symbol_table,
															code_arr,
															operation,
															params,
															IC_ind);
	}

	params_arr = parse_params(params);

	/* Operations with one parameter */
	if(addressing_modes[0] == -2){
		code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, 0, addressing_modes[1]);
		label_found = param_to_code_word(symbol_table, code_arr, params, IC_ind + 1);
		return label_found;
	}

	/* Operations with two registers as parameters */
	if(addressing_modes[0] == REG_DIRECT && addressing_modes[1] == REG_DIRECT){
		code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, addressing_modes[0], addressing_modes[1]);
		src_reg = is_register(params_arr[0]);
		dest_reg = is_register(params_arr[1]);
		code_arr[IC_ind + 1] = registers_to_code_word(src_reg, dest_reg);
		return 1;
	}

	/* Operation with two parameters where at least one of them is not a register */
	code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, addressing_modes[0], addressing_modes[1]);
	return two_params_to_code_words(symbol_table, code_arr, params, IC_ind + 1);
	
}

void print_code_arr(char **code_arr, int IC){

	int i ;
	for(i = 0; i < IC; i++)
		printf("%s\n", code_arr[i]);

}

