#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "symbol_table.h"
#include "first_pass.h"

int main(){
	
	symbolTable *symbol_table = new_symbol_table();
	int IC, IC_ind;
	int DC, DC_ind;
	int line_ind = 0;
	int error_count = 0;
	int has_symbol_def = 0;
	char *line;
	char **parsed_line;
	FILE *source;
	char filepath[] = "input.as";
	int *first_pass_output_arr;
	char **code_arr;
	char **data_arr;


	if((source = fopen(filepath, "r")) == NULL){

		error_count++;
		fprintf(stderr, "%s: error: file corrupted or not found.\n", filepath);

	}

	/* First pass */
	first_pass_output_arr = first_pass(filepath, symbol_table);
	
	error_count += first_pass_output_arr[0];
	IC = first_pass_output_arr[1];
	DC = first_pass_output_arr[2];

	printf("IC: %d, DC: %d, error_count: %d\n", IC, DC, error_count);

	/* Check for errors in first pass*/
	if(error_count == 1){
		fprintf(stderr, "%s:an error was found in first pass, couldn't proceed with second pass.\n", filepath);
		return error_count;
	
	} else if(error_count > 1){
		fprintf(stderr, "%s:%d errors were found first pass, couldn't proceed with second pass.\n", filepath, error_count);
		return	error_count;
	}

	/* Allocate memory for code and data arrays */
	code_arr = (char **)malloc(sizeof(char) * IC);
	data_arr = (char **)malloc(sizeof(char) * DC);

	IC_ind = 0;
	DC_ind = 0;
	line_ind = 0;
	line = (char *)malloc(sizeof(char) * MAX_LINE);
	while(fgets(line, sizeof(char) * MAX_LINE, source)){

		line_ind++;
		has_symbol_def = 0;
		parsed_line = parse_line(line);


		/* If line is empty or a comment, go to next line */
		if(*parsed_line[0] == '*' &&
			*parsed_line[1] == '*' &&
			*parsed_line[2] == '*')
			continue;


		if(is_operation(parsed_line[1])){

			operation_to_code_words(code_arr, parsed_line[1], parsed_line[2], IC_ind);
			IC_ind += compute_memory_for_code(parsed_line[2]);

		} else {

		}



	}

	free(first_pass_output_arr);
	fclose(source);

	return 0;
}