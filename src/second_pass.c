#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "utils.h"
#include "first_pass.h"


/* Get the file extension in a constant 
 * in order to check input file extension validity */
const char *get_file_extension(const char *filepath){
	
	const char *dot = strchr(filepath, '.');
	
	if(!dot || (dot == filepath))
		return "";

	return dot + 1;
}

/* A function to convert assembly code written in
 * the filepath and producing the output files */
int second_pass(char *filepath){
	
	int i;
	symbolTable *symbol_table = new_symbol_table();
	int IC, IC_ind; /* length of the code section */
	int DC; /* length of the data section */
	int line_ind = 0;
	int error_count = 0;
	int entries_count = 0; /* count of entry instructions */
	int *external_calls_count; /* count of calls to external variables*/
	char *line;
	char **parsed_line; /* line parsed into it code elements */
	FILE *source; /* the input file */
	int *first_pass_output_arr; /* the IC, DC and error count from the first pass */
	char **code_arr; /* The array of the machine code for the code section */
	char **data_arr; /* The array of the machine code for the data section */
	char **externals_arr; /* The array of lines to be written in the .ext file */
	char **entries_arr; /* The array of lines to be written in the .ent file */
	int label_found; /* if label isn't fount in second pass,
					  * it is not defined in code, but called for by an operation,
					  * will produce an error */
	const char *file_extension; /* the extension of the input file, to verify it is as */
	
	
	/* Check that file has as ending */
	file_extension = get_file_extension(filepath);
	if(strcmp(file_extension, "as") != 0){
		error_count++;
		fprintf(stderr, "%s: error: unknown file type, check file extension.\n", filepath);
		return error_count;
	}


	/* Open file */
	if((source = fopen(filepath, "r")) == NULL){
		error_count++;
		fprintf(stderr, "%s: error: file corrupted or not found.\n", filepath);
		return error_count;
	}

	/* First pass */
	first_pass_output_arr = first_pass(filepath, symbol_table);
	

	error_count += first_pass_output_arr[0];
	IC = first_pass_output_arr[1];
	DC = first_pass_output_arr[2];

	/* Check for errors in first pass*/
	if(error_count == 1){
		fprintf(stderr, "%s: an error was found in first pass, couldn't proceed with second pass.\n", filepath);
		return error_count;
	
	} else if(error_count > 1){
		fprintf(stderr, "%s:%d errors were found first pass, couldn't proceed with second pass.\n", filepath, error_count);
		return	error_count;
	}


	/* Allocate memory for arrays */
	code_arr = (char **)malloc(sizeof(char *) * IC);
	data_arr = (char **)malloc(sizeof(char *) * DC);
	externals_arr = (char **)malloc(sizeof(char *) * IC);
	entries_arr = (char **)malloc(sizeof(char *));

	/* Check memory allocation */
	if(code_arr == NULL || data_arr == NULL ||
		externals_arr == NULL || entries_arr == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	IC_ind = 0;
	line_ind = 0;
	line = (char *)malloc(sizeof(char) * MAX_LINE);
	external_calls_count = (int *)malloc(sizeof(int));

	/* Check memory allocation */
	if(line == NULL || external_calls_count == NULL){
		fprintf(stderr, "fatal error: program could not allocate memory.\n");
		exit(0);
	}

	(*external_calls_count) = 0;
	while(fgets(line, sizeof(char) * MAX_LINE, source)){

		line_ind++;
		parsed_line = parse_line(line);

		/* If line is empty or a comment, go to next line */
		if(*parsed_line[0] == '*' &&
			*parsed_line[1] == '*' &&
			*parsed_line[2] == '*'){
			
			free_parsed_line(parsed_line);
			continue;

		}


		if(is_operation(parsed_line[1])){
			
			label_found = operation_to_code_words(symbol_table, code_arr, parsed_line[1], parsed_line[2], IC_ind, 
				externals_arr, external_calls_count);
			
			IC_ind += compute_memory_for_code(parsed_line[2]);
			
			if(label_found == 0){
				fprintf(stderr, "%s:%d: error: label is not defined.\n", filepath, line_ind);
				error_count++;
			}
			
			free_parsed_line(parsed_line);
			continue;

		}

		if(is_instruction(parsed_line[1]) == STRING){
			string_to_code_words(symbol_table, data_arr, parsed_line[0], parsed_line[2], IC);
			
			free_parsed_line(parsed_line);
			continue;

		}

		if(is_instruction(parsed_line[1]) == DATA){
			
			data_to_code_words(symbol_table, data_arr, parsed_line[0], parsed_line[2], IC);
			free_parsed_line(parsed_line);
			continue;

		}

		if(is_instruction(parsed_line[1]) == ENTRY){
			
			entries_arr[entries_count] = write_entrie_line(symbol_table, parsed_line[2]);
			if(entries_arr[entries_count] == NULL){
				fprintf(stderr, "%s:%d: error: entrie label is not defined.\n", filepath, line_ind);
				error_count++;
			}
			entries_count++;
			free_parsed_line(parsed_line);
			continue;

		}

		free_parsed_line(parsed_line);
		
	}

	/* Write the arrays into files and free them from memory */
	if(error_count == 0){
	
		write_ob_file(filepath, code_arr, data_arr, IC, DC);
		write_entries_file(filepath, entries_arr, entries_count);
		write_externals_file(filepath, externals_arr, (*external_calls_count));
		
	}
	
	/* Free all the output arrays */
	for(i = 0; i < IC; i++)
		free(code_arr[i]);
	
	for(i = 0; i < DC; i++)
		free(data_arr[i]);

	for(i = 0; i < (*external_calls_count); i++)
		free(externals_arr[i]);

	for(i = 0; i < entries_count; i++)
		free(entries_arr[i]);

	free(code_arr);
	free(data_arr);
	free(externals_arr);
	free(entries_arr);
	free(external_calls_count);

	/* Free first pass outputs */
	free(first_pass_output_arr);
	free_symbol_table(symbol_table);

	free(line);
	
	fclose(source);
	
	return error_count;
}