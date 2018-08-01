#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "utils.h"
#include "symbol_table.h"

int main(){

	int i, line_ind, error_count;
	int IC = 0;
	int DC = 0;
	char *line;
	char **parsed_line;
	FILE *source;

	line = (char *)malloc(sizeof(char) * MAX_LINE);
	

	if((source = fopen("input.as", "r")) == NULL){
		fprintf(stderr, "%s: error: file corrupted or not found.\n", "input.as");
		exit(1);
	}

	error_count = 0;
	line_ind = 0;

	while(fgets(line, sizeof(char) * MAX_LINE, source)){
		
		line_ind++;
		parsed_line = parse_line(line);

		/* Check if line contains ilegal label*/
		if(*parsed_line[0] == '#'){
			error_count++;
			fprintf(stderr, "%s:%d: error: ilegal label definition.\n", "input.as", line_ind);
			continue;
		}

		/* Check if line contains too many arguments */
		if(*parsed_line[3] == '1'){
			error_count++;
			fprintf(stderr, "%s:%d: error: too many arguments.\n", "input.as", line_ind);
			continue;
		}


		for(i = 0; i < 4; i++)
			printf("%s\n", parsed_line[i]);
	}

	fclose(source);
	return 0;
}