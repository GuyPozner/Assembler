#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "utils.h"



int * first_pass(char *filepath, symbolTable *symbol_table){

	int *ret_arr;
	int IC = 0;
	int DC = 0;
	int line_ind, error_count;
	int has_symbol_def = 0;
	char *line;
	char **parsed_line;
	FILE *source;
	unsigned int op_code;

	
	error_count = 0;
	if((source = fopen(filepath, "r")) == NULL){

		error_count++;
		fprintf(stderr, "%s: error: file corrupted or not found.\n", filepath);

	}

	
	line_ind = 0;
	line = (char *)malloc(sizeof(char) * LINE_BUF_LEN);

	/* Check memory allocation */
	if(line == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	while(fgets(line, sizeof(char) * LINE_BUF_LEN, source)){
		
		line_ind++;
		if(strlen(line) > MAX_LINE){
			error_count++;
			fprintf(stderr, "%s:%d: error: line is longer than %d.\n", filepath, line_ind, MAX_LINE);
			continue;
		}

		if((line[0] == '\n') && (strlen(line) == 1))
			continue;
		
		has_symbol_def = 0;
		parsed_line = parse_line(line);


		/* If line is empty or a comment, go to next line */
		if(*parsed_line[0] == '*' &&
			*parsed_line[1] == '*' &&
			*parsed_line[2] == '*'){
			free_parsed_line(parsed_line);
			continue;
		}

		/* Check if line contains ilegal label*/
		if(*parsed_line[0] == '#'){
			error_count++;
			fprintf(stderr, "%s:%d: error: ilegal label definition.\n", filepath, line_ind);
			free_parsed_line(parsed_line);
			continue;
		}


		if(is_legal_label(parsed_line[0]))
			has_symbol_def = 1;

		if(is_data_storage_instruction(parsed_line[1])){
			
			/* Update the symbol table if necessery */
			if(has_symbol_def){
				if(!add_symbol(symbol_table, parsed_line[0], DC, 0, 0)){
					fprintf(stderr, "%s:%d: error: label already exists.\n", filepath, line_ind);
					error_count++;
				}
				/* Update DC according to data */
				if(is_instruction(parsed_line[1]) == DATA){
					if(compute_memory_for_data(parsed_line[2]))
						DC += compute_memory_for_data(parsed_line[2]);
					else {
						error_count++;
						fprintf(stderr, "%s:%d: error: ilegal data arguments.\n", filepath, line_ind);
					}
				/* Update DC according to string */
				} else {

					if(compute_memory_for_string(parsed_line[2]))
						DC += compute_memory_for_string(parsed_line[2]);
					else {

						error_count++;
						fprintf(stderr, "%s:%d: error: ilegal string arguments.\n", filepath, line_ind);
					}
				}
				
			} else {
				error_count++;
				fprintf(stderr, "%s:%d: error: label name is not defined for data storage.\n", filepath, line_ind);
			}
			free_parsed_line(parsed_line);
			continue;
			
		}
		
		if(is_data_definition_instruction(parsed_line[1])){
			if(is_instruction(parsed_line[1]) == EXTERN){
				if(is_legal_label(parsed_line[2]) == LEGAL_LABEL){
					
					if(add_symbol(symbol_table, parsed_line[2], DC, 1, 0) == 0){
						error_count++;
						fprintf(stderr, "%s:%d: error: label already exist.\n", filepath, line_ind);
					}

				} else {
					
					/* Ilegal label */
					error_count++;
					fprintf(stderr, "%s:%d: error: ilegal argument for %s.\n", filepath, line_ind, parsed_line[1]);
				}
			}
			
			/* Check for errors in .entry arguments */
			if(is_instruction(parsed_line[1]) == ENTRY &&
			 is_legal_label(parsed_line[2]) != LEGAL_LABEL){
				/* Ilegal label */
				error_count++;
				fprintf(stderr, "%s:%d: error: ilegal argument for %s.\n", filepath, line_ind, parsed_line[1]);	
			}

			free_parsed_line(parsed_line);
			continue;
		}


		if((op_code = is_operation(parsed_line[1]))){

			if(has_symbol_def){

				if(add_symbol(symbol_table, parsed_line[0], IC, 0, 1) == 0){
					
					error_count++;
					fprintf(stderr, "%s:%d: error: label already exist.\n", filepath, line_ind);
				}
			}

			
			/* Compute the number of memory words */
			if((op_code == STOP ||
				op_code == RTS) &&
				(*(parsed_line[2]) == '*')){
				
				IC++;

			} else if (is_legal_addressing_modes(parsed_line[1], parsed_line[2])){
				
				IC += compute_memory_for_code(parsed_line[2]);
				
			} else {
				
				fprintf(stderr, "%s:%d: error: ilegal arguments to operation %s.\n", filepath, line_ind, parsed_line[1]);
				error_count++;

			}
		free_parsed_line(parsed_line);
		continue;


		} else {

			error_count++;
			fprintf(stderr, "%s:%d: error: unknown operation.\n", "inpus.as", line_ind);
			free_parsed_line(parsed_line);
		}
		
	}
	

	ret_arr = (int *)malloc(sizeof(int) * 3);
	/* Check memory allocation */
	if(ret_arr == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	update_adresses(symbol_table, IC);
	ret_arr[0] = error_count;
	ret_arr[1] = IC;
	ret_arr[2] = DC;

	fclose(source);
	free(line);

	return ret_arr;
}