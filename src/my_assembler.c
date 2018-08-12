#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "second_pass.h"


int main(int argc, char *argv[]){

	int i;
	int error_count = 0;

	for(i = 1; i < argc; i++){
		error_count = second_pass(argv[i]);
		if(error_count)
			printf("%s: errors were found in second pass, cannot create output files.\n", argv[i]);
	}


	return 0;
}