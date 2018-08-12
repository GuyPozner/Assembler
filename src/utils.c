#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "utils.h"

/* Counts the number of digits in an integer */
int digit_count(int n){
	
	int count = 0;
	
	while(n != 0){
		n /= 10;
		count ++;
	}
	
	return count;
}

/* Converts an integer to char array */
char *itoa(int val, int base){

	static char buf[32] = {0};

	int i = 30;
	for(;val && i; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i + 1];

}

/* Returns TRUE if c is in word, FALSE otherwise */
int contains_char(char *word, char c){

	int i;

	for(i = 0; word[i]; i++){
		if(word[i] == c)
			return TRUE;
	}

	return FALSE;
}

/* Returns label status for line */
int contains_legal_label(char *word){

	int i;
	int ilegal_char = FALSE;

	if(!contains_char(word, ':'))
		return NO_LABEL;

	for(i = 0; word[i] != ':'; i++)
		if((word[i] < '0') || (word[i] > 'z'))
			ilegal_char = TRUE;
	
	if(ilegal_char)
		return ILEGAL_LABEL;

	return LEGAL_LABEL;
}

/* Return FALSE if word is
 * an ilegal label, TRUE otherwise */
int is_legal_label(char *word){

	int i;

	/* Check that it is not an operation,
	 * instruction or a register */
	if(is_operation(word) ||
		is_instruction(word) ||
		(is_register(word) != -1))
		return FALSE;


	for(i = 0; word[i]; i++)
		if((word[i] < '0') || (word[i] > 'z'))
			return FALSE;
	
	return TRUE;
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
	int hob_barckets = 0;
	int count = 0;

	for(i = 0; word[i]; i++){
		
		if(word[i] == ')'){
			count--;
			hob_barckets = 1;
		}
		
		if(count < 0)
			break;

		if(word[i] == '('){
			count++;
			hob_barckets = 1;
		}
	}

	/* Legal brackets */
	if(hob_barckets && (count == 0))
		return LEGAL_BRACKETS;

	/* Ilegal brackets */
	else if(hob_barckets && (count != 0))
		return ILEGAL_LABEL;

	/* No brackets */
	else
		return NO_BRACKETS;

}


/* Returns the number of register if legal register,
 * 0 otherwise. */
int is_register(char *word){

	
	if(strlen(word) == 1)
		return -1;
	

	if((word[0] == 'r') &&
		('0' <= word[1]) && 
		(word[1] <= ('0' + MAX_REG)) &&
		(strlen(word) == 2))
		return (word[1] - '0');
	
	return -1;
}


/* Return TRUE if operation takes no arguments, 
 * FALSE otherwise */
int is_no_operand_operation(char *operation){
	
	int op_num = is_operation(operation);

	if(op_num == RTS ||
		op_num == STOP)
		return TRUE;
	
	return FALSE;
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
	
	/* Check memory allocation */
	if(parsed_line == NULL ||
		label == NULL ||
		operation_instruction == NULL ||
		params == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	/* Line is commented out */
	if(line[0] == ';'){
		label[0] = '*';
		operation_instruction[0] = '*';
		params[0] = '*';

	} else {

		memmove(tmp_str, line, strlen(line) + 1);
		
		/* line starts with label */
		if(contains_legal_label(tmp_str) == 1){
			
			tmp = strtok(tmp_str, ":");
			memmove(label, tmp, strlen(tmp) + 1);
			

			if((strlen(label) <= MAX_LABEL) &&
				(tmp = strtok(NULL, " \t\n")) != NULL){
				
				memmove(tmp_str, tmp, strlen(tmp) + 1);
				if(is_operation(tmp_str) || is_instruction(tmp_str))
					memmove(operation_instruction, tmp_str, strlen(tmp_str) + 1);
				
				else 
					operation_instruction[0] = '#';
				
				
				if(((tmp = strtok(NULL, "\n")) != NULL) &&
					(strlen(tmp) <= (MAX_PARAMS_LEN)))
						
						memmove(params, tmp, strlen(tmp) + 1);		
				else {
					if(is_no_operand_operation(tmp_str))
						params[0] = '*';
					else
						params[0] = '#';
				}
				
			} else {
				operation_instruction[0] = '#';
				params[0] = '#';
			}
			

		/* Line starts with operation or instruction */
		} else {

			if(contains_legal_label(tmp_str) == 0)
				label[0] = '*';
			else
				label[0] = '#';
				
			
			/* Line has more than one word */
			if(((tmp = strtok(tmp_str, " \t\n")) != NULL) &&
			 (strlen(tmp) <= MAX_OP_INST_LEN)){
				
				memmove(tmp_str, tmp, strlen(tmp) + 1);
				if(is_operation(tmp_str) || is_instruction(tmp_str))
					memmove(operation_instruction, tmp_str, strlen(tmp_str) + 1);
				else 
					operation_instruction[0] = '#';
				
				
				if(((tmp = strtok(NULL, "\n")) != NULL) &&
					((strlen(tmp)) <= MAX_PARAMS_LEN))

					memmove(params, tmp, strlen(tmp) + 1);
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

/* Return TRUE if word is a data storage instruction, 
 * FALSE otherwise*/
int is_data_storage_instruction(char *word){

	int instruction = is_instruction(word);

	if(instruction == DATA || instruction == STRING)
		return TRUE;

	return FALSE;
}

/* Return TRUE if word is a data definition instruction, 
 * FALSE otherwise*/
int is_data_definition_instruction(char *word){

	int instruction = is_instruction(word);

	if(instruction == EXTERN || instruction == ENTRY)
		return TRUE;

	return FALSE;
}

/* Return TRUE if operation can accept 2 arguments, 
 * FALSE otherwise */
int is_two_operands_operation(char *operation){
	
	int op_num = is_operation(operation);

	if(op_num == MOV ||
		op_num == CMP ||
		op_num == ADD ||
		op_num == SUB ||
		op_num == LEA)
		return TRUE;

	return FALSE;
}

/* Return TRUE if operation accept 1 argument, 
 * FALSE otherwise */
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
		return TRUE;
	
	return FALSE;
}



/* Return TRUE if operation can take jump parameters, 
 * FALSE otherwise */
int legal_operation_jmp_adressing_with_params(char *operation){
	int op_num = is_operation(operation);

	if(op_num == JMP ||
		op_num == JSR ||
		op_num == BNE)
		return TRUE;
	
	return FALSE;
}


/* Returns the index of the first left bracket in params */
int first_left_bracket_ind(char *params){

	int i = 0;
	while(params[i] != '(')
		++i;

	return i;
}

/* Returns a char pointer to a freshly allocated
 * subset of str which start at start and ends at end */
char * substr(char *str, int start, int end){

	char *sub_str;
	sub_str = (char *)malloc(sizeof(char) * ((end - start) + 1));
	
	/* Check memory allocation */
	if(sub_str == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	memcpy(sub_str, str + start, end - start);
	sub_str[(end - start)] = '\0';
	
	return sub_str;

}


/* Returns the label from the parametes 
 * of a jump param operation */
char * get_label_from_jmp_params(char *params){
	if(contains_char(params, '('))
		return substr(params, 0, first_left_bracket_ind(params));

	return NULL;

}


/* Returns a char pointer to the parametes inside
 * the brackets of a jump parameters operation */
char * get_params_from_jmp_params(char *params){
	char *jmp_params;
	jmp_params = substr(params, first_left_bracket_ind(params) + 1, 
		strlen(params) - 1);
	return jmp_params;
}


/* Returns TRUE if cnst is a legal constant,
 * FALSE otherwise */
int is_legal_constant(char *cnst){

	int i;

	/* cnst is empty */
	if(*(cnst + 1) == 0)
		return FALSE;

	if(cnst[0] != '#')
		return FALSE;

	if(cnst[1] == '-' || cnst [1] == '+')
		i = 2;
	else
		i = 1;


	for(;cnst[i];i++)
		if(cnst[i] <= '0' || cnst[i] >= '9')
			return FALSE;

	return TRUE;
}


/* Returns TRUE if param is a legal param,
 * FALSE otherwise */
int is_legal_param(char *param){
	
	if(is_legal_label(param) ||
		is_legal_constant(param) ||
		(is_register(param) != -1))
		return TRUE;
	
	return FALSE;
}

/* Returns TRUE if params are legal jump params,
 * FALSE otherwise */
int is_legal_jmp_adressing_with_params_mode(char *params){

	char *label;
	char *in_brackets;
	char *rhs_param, *lhs_param;
	
	/* Empty params */
	if(strlen(params) == 1)
		return FALSE;


	label = get_label_from_jmp_params(params);
	if(label == NULL)
		return FALSE;

	if(!is_legal_label(label)){
		free(label);
		return FALSE;
	}

	if(contains_brackets(params) != 1)
		return FALSE;
	
	in_brackets = substr(params,
						first_left_bracket_ind(params) + 1,
						strlen(params) - 1);
	if(contains_char(in_brackets, ',')){
		
		if(((lhs_param = strtok(in_brackets, ",")) != NULL) &&
			((rhs_param = strtok(NULL, ",")) != NULL)){
			if(is_legal_param(lhs_param) && is_legal_param(rhs_param)){
				
				free(label);
				free(in_brackets);
				return TRUE;
			}
		}

	}
	
	free(label);
	free(in_brackets);
	return FALSE;

}


/* Returns a number corresponding 
 * with the addressing mode */
int addressing_mode(char *param){
	
	
	if(is_register(param) != -1)
		return REG_DIRECT;

	if(is_legal_label(param))
		return DIRECT;
	
	if(is_legal_constant(param))
		return IMMEDIATE;
	
	if(is_legal_jmp_adressing_with_params_mode(param))
		return JMP_PARAMS;
	
	return ILEGAL_ADDRESSING_MODE;
}


/* Return a integer pointer with the addressing modes,
 * Allocated to two position, since the maximum params are 2 */
int * params_to_addressing_modes(char *params){

	int *addressing_modes;
	char *rhs_param, *lhs_param;
	char *tmp_params;
	
	addressing_modes = (int *)malloc(sizeof(int) * 2);
	/* Check memory allocation */
	if(addressing_modes == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	if((strlen(params) == 1) && !(is_legal_label(params))){
		addressing_modes[0] = NO_PARAM;
		addressing_modes[1] = NO_PARAM;
		return addressing_modes;
	}
	
	
	tmp_params = (char *)malloc(sizeof(char) * (strlen(params) + 1));
	
	/* Check memory allocation */
	if(tmp_params == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	memmove(tmp_params, params, strlen(params) + 1);
	
	/* If two params */
	if(contains_char(params, ',') && (!(contains_char(params, '(')))){
		if((lhs_param = strtok(tmp_params, " ,\t")) != NULL &&
			(rhs_param = strtok(NULL, " ,\t")) != NULL){


			addressing_modes[0] = addressing_mode(lhs_param);
			addressing_modes[1] = addressing_mode(rhs_param);
			

			free(tmp_params);
			return addressing_modes;
		} else {
			addressing_modes[0] = ILEGAL_ADDRESSING_MODE;
			addressing_modes[1] = ILEGAL_ADDRESSING_MODE;
			free(tmp_params);
			return addressing_modes;
		}
	} else {
		addressing_modes[0] = NO_PARAM;
		addressing_modes[1] = addressing_mode(params);
		free(tmp_params);
		return addressing_modes;
	}
}


/* Returns an array with two position containing
 * the addressing modes for the jump parameters */
int * get_addressing_modes_from_jmp_params(char *params){
	char *jmp_params;
	int *addressing_modes;
	jmp_params = substr(params,
						first_left_bracket_ind(params) + 1, 
						(strlen(params) - 1));

	
	addressing_modes = params_to_addressing_modes(jmp_params);
	free(jmp_params);
	return addressing_modes;
}


/* Returns TRUE if addressing mode of params 
 * can be used with operation, FALSE otherwise */
int is_legal_addressing_modes(char *operation, char *params){

	int op_num = is_operation(operation);
	int *addressing_modes;

	
	addressing_modes = params_to_addressing_modes(params);
	
	if(op_num == MOV || op_num == ADD || op_num == SUB)
		if((addressing_modes[0] == IMMEDIATE ||
			addressing_modes[0] == DIRECT ||
			addressing_modes[0] == REG_DIRECT) &&
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT)){
			
			free(addressing_modes);
			return TRUE;
	}

	if(op_num == CMP)
		if((addressing_modes[0] == IMMEDIATE ||
			addressing_modes[0] == DIRECT ||
			addressing_modes[0] == REG_DIRECT) &&
			(addressing_modes[1] == IMMEDIATE ||
			 addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT)){
			
			free(addressing_modes);
			return TRUE;
	}

	if(op_num == NOT || op_num == CLR || op_num == INC ||
	 op_num == DEC || op_num == RED)
		if((addressing_modes[0] == NO_PARAM) &&
			(addressing_modes[1] == DIRECT ||
			addressing_modes[1] == REG_DIRECT)){
			
			free(addressing_modes);
			return TRUE;
		}

	if(op_num == LEA)
		if((addressing_modes[0] == DIRECT) &&
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT)){
			
			free(addressing_modes);
			return TRUE;
	}

	if(op_num == JMP || op_num == BNE || op_num == JSR)
		if((addressing_modes[0] == NO_PARAM) &&
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT ||
			 addressing_modes[1] == JMP_PARAMS)){
			
			free(addressing_modes);
			return TRUE;
	}

	if(op_num == PRN)
		if((addressing_modes[0] == NO_PARAM) && 
			(addressing_modes[1] == DIRECT ||
			 addressing_modes[1] == REG_DIRECT ||
			 addressing_modes[1] == IMMEDIATE)){
			
			free(addressing_modes);
			return TRUE;
		}

	free(addressing_modes);
	return FALSE;
	
}

/* Returns the memory size needed to store 
 * integer with the .data instruction */
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

/* Returns the memory size needed to store 
 * characters array with the .string instruction */
int compute_memory_for_string(char *params){

	if(params[0] != '"' ||
		params[strlen(params) - 1] != '"')
		return 0;
	else
		/* An extra space is added for the 0 terminating the string */
		return strlen(params) - 1;
	
}


/* Returns the memory size needed to store 
 * the code */
int compute_memory_for_code(char *params){
	
	int *addressing_modes, *jmp_addressing_modes;
	int memory_size = 1;

	addressing_modes = params_to_addressing_modes(params);

	/* If no params */
	if(addressing_modes[0] == -2 && addressing_modes[1] == -2){
		free(addressing_modes);
		return 1;
	}

	/* Both params are registers*/
	if(addressing_modes[0] == REG_DIRECT &&
		addressing_modes[1] == REG_DIRECT)
		memory_size++;

	/* Jmp addressing with params */
	else if(addressing_modes[0] == -2 && addressing_modes[1] == JMP_PARAMS){
		
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

	free(addressing_modes);
	return memory_size;
}



/* Returns an integer pointer for the array
 * of integers given by .data instruction */
int * parse_data_str_to_data_int_arr(char *data_str){
	
	int i;
	char *tmp_str;
	int * data_int;
	char *tmp_data_str;
	int memory_size = compute_memory_for_data(data_str);
	/* 3 times the size to allow - + and , to be copied */
	tmp_data_str = (char *)malloc(sizeof(char) * (memory_size * 3));
	
	/* Check memory allocation */
	if(tmp_data_str == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	strcpy(tmp_data_str, data_str);

	data_int = (int *)malloc(sizeof(int) * memory_size);
	
	/* Check memory allocation */
	if(data_int == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	tmp_str = strtok(tmp_data_str, " ,\t");
	data_int[0] = atoi(tmp_str);
	for(i = 1; i < memory_size; i++){
		tmp_str = strtok(NULL, " ,\t");
		data_int[i] = atoi(tmp_str);
	}

	free(tmp_data_str);
	return data_int;
}


/* Returns char * to the an int converted to wierd
 * base 2, it also allocates it with malloc,
 * should keep track in order to free the memory */
char * int_to_wierd_base2(int num, int len){
	int i;
	char *base2_str;
	base2_str = (char *)malloc(sizeof(char) * (len + 1));

	/* Check memory allocation */
	if(base2_str == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

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


/* Returns a char * to a char in wierd base 2 */
char * char_to_wierd_base2(char c){

	return int_to_wierd_base2((int)c, 14);
}


/* Converts a line containing operation to a code word,
 * allocates memory, should keep track
 * of the memory_word pointer */
char * operation_to_code_word(int param1, int param2, int op_code,
							 int src_addressing, int dest_addressing){

	char *memory_word;
	char *base2_str;
	
	memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));
	
	/* Check memory allocation */
	if(memory_word == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	memory_word[0] = '\0';

	strcat(memory_word, (base2_str = int_to_wierd_base2(param1, 2)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(param2, 2)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(op_code, 4)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(src_addressing, 2)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(dest_addressing, 2)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(ABS, 2)));
	free(base2_str);

	return memory_word; 

}


/* Writes a line for the .ext file,
 * allocates memory for that line, 
 * should keep track of its output in
 * order to free */
char * write_external_line(char *label, int mem_adress){

	char *external_line;

	external_line = (char *)malloc(sizeof(char) * 
		(MAX_LABEL + 2 + digit_count(mem_adress)));

	/* Check memory allocation */
	if(external_line == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	strcpy(external_line, label);
	strcat(external_line, "\t");
	strcat(external_line, itoa(mem_adress, 10));

	return external_line;

}


/* Writes a line for the .ent file,
 * allocates memory for that line, 
 * should keep track of its output in
 * order to free */
char *write_entrie_line(symbolTable *symbol_table, char *label){

	char *entrie_line;
	int mem_adress;

	mem_adress = get_symbol_adress(symbol_table, label);
	if(mem_adress == -1)
		return NULL;


	entrie_line = (char *)malloc(sizeof(char) * 
		(MAX_LABEL + 2 + digit_count(mem_adress)));

	/* Check memory allocation */
	if(entrie_line == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	strcpy(entrie_line, label);
	strcat(entrie_line, "\t");
	strcat(entrie_line, itoa(mem_adress, 10));
	

	return entrie_line;

}

/* Turns a label parameter into a code word,
 * when label is external it creates as a side effect
 * a line in the externals array, should keep track
 * of its output since it allocates memory
 * for the memory word */
char * label_to_code_word(symbolTable *symbol_table, char *label, int IC,
 char **externals_arr, int *external_calls_count){

	char *memory_word;
	int adress;
	int is_external;
	char *base2_str;


	memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));
	
	/* Check memory allocation */
	if(memory_word == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	memory_word[0] = '\0';

	if((adress = get_symbol_adress(symbol_table, label)) == -1){
		strcat(memory_word, (base2_str = int_to_wierd_base2(0, 14)));
		free(base2_str);
		return memory_word;
	}

	is_external = is_symbol_external(symbol_table, label);
	
	if(is_external){

		/* Update external arrays */
		externals_arr[(*external_calls_count)++] = write_external_line(label, IC + MEM_ADRESS);
		
		strcat(memory_word, (base2_str = int_to_wierd_base2(0, 12)));
		free(base2_str);
		strcat(memory_word, (base2_str = int_to_wierd_base2(EXT, 2)));
		free(base2_str);
	} else {
		
		strcat(memory_word, (base2_str = int_to_wierd_base2(adress, 12)));
		free(base2_str);
		strcat(memory_word, (base2_str = int_to_wierd_base2(RELOC, 2)));
		free(base2_str);
	}

	return memory_word; 

}


/* Returns a char * to the code word
 * representing registers src_reg and dest_reg
 * should keep track of its output, since it
 * allocates memory for the code word */
char * registers_to_code_word(int src_reg, int dest_reg){

	char *memory_word;
	char *base2_str;
	memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));
	
	/* Check memory allocation */
	if(memory_word == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	memory_word[0] = '\0';

	strcat(memory_word , (base2_str = int_to_wierd_base2(src_reg, 6)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(dest_reg, 6)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(ABS, 2)));
	free(base2_str);
	return memory_word;
}

/* Returns a char * to the code word
 * representing registers a constant,
 * should keep track of its output, since it
 * allocates memory for the code word */
char * constant_to_code_word(int cnst){

	char *memory_word;
	char *base2_str;

	memory_word = (char *)malloc(sizeof(char) * (MEMORY_WORD_LEN + 1));
	
	/* Check memory allocation */
	if(memory_word == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	memory_word[0] = '\0';

	strcat(memory_word, (base2_str = int_to_wierd_base2(cnst, 12)));
	free(base2_str);
	strcat(memory_word, (base2_str = int_to_wierd_base2(ABS, 2)));
	free(base2_str);

	return memory_word;
}

/* Parse parametes into an array of character arrays
 * with two positions, one for the source paramters
 * and one for the destination parameter, sholud
 * keep track of its output since it allocates memory */
char **parse_params(char *params){

	
	char **params_arr;
	char *tmp_params;
	char *tmp;

	params_arr = (char **)malloc(sizeof(char *) * 2);
	tmp_params = (char *)malloc(sizeof(char) * (strlen(params) + 1));
	
	params_arr[0] = (char *)malloc(sizeof(char) * (strlen(params) + 1));
	params_arr[1] = (char *)malloc(sizeof(char) * (strlen(params) + 1));

	/* Check memory allocation */
	if(params_arr == NULL ||
		tmp_params == NULL ||
		params_arr[0] == NULL ||
		params_arr[1] == NULL){
		
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	/* Use memove in order to avoid 
	 * source and destination overlap */
	memmove(tmp_params, params, strlen(params) + 1);
	tmp = strtok(tmp_params, " ,\t");
	memmove(params_arr[0], tmp, strlen(tmp) + 1);
	if((tmp = strtok(NULL, " ,\t")) != NULL){
		memmove(params_arr[1], tmp, strlen(tmp) + 1);
	}
	else {

		memmove(params_arr[1], params_arr[0], strlen(params_arr[0]) + 1);
		memmove(params_arr[0], "0", 2);
		
	}
	
	free(tmp_params);
	return params_arr;
}


/* Return an integer which is representes by
 * the char array cnst */
int constant_str_to_int(char *cnst){

	char *str;
	int retval = (int) strtol((cnst+1) , &str, 10);

	return retval;

}

/* Converts a parameter into a code word
 * and writes it to the IC_ind position
 * in the code array, Returns FALSE if
 * label was called but not found, TRUE otherwise */
int param_to_code_word(symbolTable *symbol_table, char **code_arr,
 							char *param, int IC_ind, int is_src, char **externals_arr, int *external_calls_count){
	

	char *code_word;
	int label_found = FALSE;

	/* Param is register */
	if(0 <= is_register(param) && is_register(param) <= MAX_REG){
		if(is_src)
			code_arr[IC_ind] = registers_to_code_word(is_register(param), 0);
		else
			code_arr[IC_ind] = registers_to_code_word(0, is_register(param));
		
		
		label_found = TRUE;
	}
	
	/* Param is constant */
	 else if(is_legal_constant(param)){
		code_arr[IC_ind] = constant_to_code_word(constant_str_to_int(param));
		
		label_found = TRUE;

	/* Param is label */
	} else {
		
		code_word = label_to_code_word(symbol_table, param, IC_ind, externals_arr, external_calls_count);
		code_arr[IC_ind] = code_word;

		if(strcmp(code_word, EMPTY_WORD) == 0)
			label_found = FALSE;
		else
			label_found = TRUE;
		
	}

	return label_found;
}


/* Converts two parameters into code word\s
 * and writes it to the IC_ind position
 * in the code array, Returns FALSE if
 * label was called but not found, TRUE otherwise */
int two_params_to_code_words(symbolTable *symbol_table, char **code_arr,
 							char *params, int IC_ind, char **externals_arr, int *external_calls_count){

	int label_found;
	char **params_arr = parse_params(params);
	
	label_found = param_to_code_word(symbol_table, code_arr, params_arr[0], IC_ind, 1, externals_arr, external_calls_count);
	label_found += param_to_code_word(symbol_table, code_arr, params_arr[1], IC_ind + 1, 0, externals_arr, external_calls_count);
	
	free(params_arr[0]);
	free(params_arr[1]);
	free(params_arr);

	/* if label_found != 2 it means that 
	 * a label was searched for but not found */
	if(label_found != 2)
		return FALSE;
	
	return TRUE;
}


/* Converts jump parameters into code word\s
 * and writes it to the IC_ind position
 * in the code array, Returns FALSE if
 * label was called but not found, TRUE otherwise */
int operation_with_jmp_addressing_to_code_words(symbolTable *symbol_table, char **code_arr,char *operation,
 							char *params, int IC_ind, char **externals_arr, int *external_calls_count){

	int src_reg, dest_reg;
	int op_code = is_operation(operation) - 1;
	int *jmp_addressing_modes = get_addressing_modes_from_jmp_params(params);
	int param1 = 0;
	int param2 = 0;
	char *label = get_label_from_jmp_params(params);
	char *jmp_params = get_params_from_jmp_params(params);
	char **jmp_params_arr = parse_params(jmp_params);
	int label_found;

	param1 = jmp_addressing_modes[0];
	param2 = jmp_addressing_modes[1];
	
	code_arr[IC_ind] = operation_to_code_word(param1, param2, op_code, 0, JMP_PARAMS);
	code_arr[IC_ind + 1] = label_to_code_word(symbol_table, label, IC_ind + 1, externals_arr, external_calls_count);
	
	/* The return value, detect labels which aren't defined */
	label_found = (strcmp(code_arr[IC_ind + 1], EMPTY_WORD) != 0);

	if(param1 == REG_DIRECT && param2 == REG_DIRECT){

		src_reg = is_register(jmp_params_arr[0]);
		dest_reg = is_register(jmp_params_arr[1]);
		code_arr[IC_ind + 2] = registers_to_code_word(src_reg, dest_reg);
		
		free(jmp_params_arr[0]);
		free(jmp_params_arr[1]);
		free(jmp_params_arr);
		free(jmp_addressing_modes);
		free(jmp_params);
		free(label);

		return label_found;

	} else {
		if(!(two_params_to_code_words(symbol_table, code_arr, jmp_params, IC_ind + 2, externals_arr, external_calls_count))){
			
			free(jmp_params_arr[0]);
			free(jmp_params_arr[1]);
			free(jmp_params_arr); 
			free(jmp_addressing_modes);
			free(jmp_params);
			free(label);

			return label_found;
		}
		else{
			
			free(jmp_params_arr[0]);
			free(jmp_params_arr[1]);
			free(jmp_params_arr);
			free(jmp_addressing_modes);
			free(jmp_params);
			free(label);
			return label_found;
		}
	}
}


/* The API function to write an convert an operation to
 * the corresponding code word, Returns FALSE if label was
 * searched and not found, TRUE otherwise. */
int operation_to_code_words(symbolTable *symbol_table, char **code_arr,char *operation,
 							char *params, int IC_ind, char **externals_arr, int *external_calls_count){


	int label_found;
	int *addressing_modes;
	int src_reg, dest_reg;
	int op_code = is_operation(operation) - 1; /* The operation enum is offset in order to return true value */
	char **params_arr;

	/* If operation has no params */
	if((op_code + 1) == STOP || (op_code + 1) == RTS){
		code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, 0, 0);
		label_found = 1;
		return TRUE;
	}

	addressing_modes = params_to_addressing_modes(params);

	/* Addressing mode jmp with params */
	if(addressing_modes[0] == -2 && addressing_modes[1] == JMP_PARAMS){
		free(addressing_modes);
		return operation_with_jmp_addressing_to_code_words(symbol_table,
															code_arr,
															operation,
															params,
															IC_ind,
															externals_arr,
															external_calls_count);
	}
	
	
	params_arr = parse_params(params);

	/* Operations with one parameter */
	if(addressing_modes[0] == -2){
		code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, 0, addressing_modes[1]);
		label_found = param_to_code_word(symbol_table, code_arr, params, IC_ind + 1, 0, externals_arr, external_calls_count);
		
	}

	/* Operations with two registers as parameters */
	else if(addressing_modes[0] == REG_DIRECT && addressing_modes[1] == REG_DIRECT){
		code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, addressing_modes[0], addressing_modes[1]);
		src_reg = is_register(params_arr[0]);
		dest_reg = is_register(params_arr[1]);
		code_arr[IC_ind + 1] = registers_to_code_word(src_reg, dest_reg);
		label_found = TRUE;

	/* Operation with two parameters where at least one of them is not a register */
	} else {

		code_arr[IC_ind] = operation_to_code_word(0, 0, op_code, addressing_modes[0], addressing_modes[1]);
		label_found = two_params_to_code_words(symbol_table, code_arr, params, IC_ind + 1, externals_arr, external_calls_count);
	
	}

	free(addressing_modes);
	free(params_arr[0]);
	free(params_arr[1]);
	free(params_arr);

	return label_found;
	
}


/* Writes a string instruction into the data_arr,
 * Returns FALSE if symbol is external, TRUE otherwise */
int string_to_code_words(symbolTable *symbol_table,
 char **data_arr, char *label, char *str, int IC){

	int i;
	int symbol_adress;
	int memory_size = compute_memory_for_string(str);

	if(is_symbol_external(symbol_table, label))
		return FALSE;

	
	symbol_adress = get_symbol_adress(symbol_table, label);
	for(i = 0; i < (memory_size - 1); i++)
		data_arr[symbol_adress + i - IC - MEM_ADRESS] = char_to_wierd_base2(str[i + 1]);

	data_arr[symbol_adress + i - IC - MEM_ADRESS] = int_to_wierd_base2(0, 14);
	
	return TRUE;
}


/* Writes a data instruction into the data_arr,
 * Returns FALSE if symbo is external, TRUE otherwise */
int data_to_code_words(symbolTable *symbol_table, 
	char **data_arr, char *label, char *data_str, int IC){
	
	int i;
	int symbol_adress;
	int *data_int = parse_data_str_to_data_int_arr(data_str);
	int memory_size = compute_memory_for_data(data_str);

	if(is_symbol_external(symbol_table, label))
		return FALSE;

	
	symbol_adress = get_symbol_adress(symbol_table, label);
	for(i = 0; i < memory_size; i++){
		
		data_arr[symbol_adress + i - IC - MEM_ADRESS] = char_to_wierd_base2(data_int[i]);
		
	}
	
	free(data_int);

	return TRUE;
}

/* Writes the .ob file into filepath,
 * Returns BAD_FILE if the file is currepted,
 * or loaded by another program, GOOD_FILE otherwise */
int write_ob_file(char *filepath, char **code_arr, char **data_arr, int IC, int DC){

	int i;
	char * ob_filepath;
	FILE *ob_file;
	
	/* Create the .ob file */
	ob_filepath = (char *)malloc(sizeof(char) * (strlen(filepath) + 2));
	
	/* Check memory allocation */
	if(ob_filepath == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	strcpy(ob_filepath, filepath);
	ob_filepath = strtok(ob_filepath, ".");
	strcat(ob_filepath, ".ob\0");
	
	ob_file = fopen(ob_filepath, "w");
	/* Cannot create file */
	if(ob_file == NULL){
		fprintf(stderr, "error: cannot write into %s\n", ob_filepath);
		return BAD_FILE;
	}
	

	/* print contents into the file */
	fprintf(ob_file, "\t%d\t%d\n", IC, DC);
	for(i = 0; i < IC; i++)
		fprintf(ob_file, "%04d\t%s\n", i + MEM_ADRESS, code_arr[i]);
	
	for(i = 0; i < DC; i++)
		fprintf(ob_file, "%04d\t%s\n", i + IC + MEM_ADRESS, data_arr[i]);
		
	free(ob_filepath);
	fclose(ob_file);

	return GOOD_FILE;
}


/* Writes the .ent file into filepath,
 * Returns BAD_FILE if the file is currepted,
 * or loaded by another program, GOOD_FILE otherwise */
int write_entries_file(char *filepath, char **entries_arr, int entries_count){

	int i;
	char * ent_filepath;
	FILE *ent_file;
	
	/* Create the .ent file */
	ent_filepath = (char *)malloc(sizeof(char) * (strlen(filepath) + 2));
	
	/* Check memory allocation */
	if(ent_filepath == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	strcpy(ent_filepath, filepath);
	ent_filepath = strtok(ent_filepath, ".");
	strcat(ent_filepath, ".ent\0");
	
	ent_file = fopen(ent_filepath, "w");
	/* Cannot create file */
	if(ent_file == NULL){
		fprintf(stderr, "error: cannot write into %s\n", ent_filepath);
		return BAD_FILE;
	}
	

	/* print contents into the file */
	for(i = 0; i < entries_count; i++)
		fprintf(ent_file, "%s\n", entries_arr[i]);
	
	free(ent_filepath);
	fclose(ent_file);
	
	return GOOD_FILE;
}


/* Writes the .ext file into filepath,
 * Returns BAD_FILE if the file is currepted,
 * or loaded by another program, GOOD_FILE otherwise */
int write_externals_file(char *filepath, char **externals_arr, int external_calls_count){

	int i;
	char * ext_filepath;
	FILE *ext_file;
	
	/* Create the .ent file */
	ext_filepath = (char *)malloc(sizeof(char) * (strlen(filepath) + 2));
	strcpy(ext_filepath, filepath);
	ext_filepath = strtok(ext_filepath, ".");
	strcat(ext_filepath, ".ext\0");
	
	ext_file = fopen(ext_filepath, "w");
	/* Cannot create file */
	if(ext_file == NULL){
		fprintf(stderr, "error: cannot write into %s\n", ext_filepath);
		return BAD_FILE;
	}

	/* print contexts into the file */
	for(i = 0; i < external_calls_count; i++)
		fprintf(ext_file, "%s\n", externals_arr[i]);
		
	free(ext_filepath);
	fclose(ext_file);

	return GOOD_FILE;
}


/* frees the parsed line */
void free_parsed_line(char **parsed_line){
	int i;
	for(i = 0; i < 3; i++)
		free(parsed_line[i]);
	free(parsed_line);
}


