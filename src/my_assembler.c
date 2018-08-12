#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "second_pass.h"


int main(int argc, char *argv[]){

	int i;
	int error_count = 0;

	for(i = 1; i < argc; i++){
		error_count = second_pass(argv[i]);
		
		/* Print error message */
		if(error_count == 1)
			printf("%s: 1 error was found, cannot create output files.\n", argv[i]);
		else if(error_count > 1)
			printf("%s: %d errors were found, cannot create output files.\n", argv[i], error_count);
	
	}

	return 0;
}