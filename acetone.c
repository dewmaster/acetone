#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "truth.h"

int main(int argc, char *argv[]){

	
	/*
		In the future, it would be good to add multiple modes,
		one for generating a blank input file, one for only specifying 
		the ones.
	
	*/

	//Parse commandline input and file information.
	FILE * input;

	input = fopen(argv[1],"r");
	char cur = getc(input);
	int row = 0;
	char ** table = malloc(sizeof(char *));
	int rowmax = 0;
	char **IOnames = malloc(sizeof(char *));
	int names = 0;

	//Read input file
	while(cur != EOF){
		//Beyond the first row, the array holding the values needs to be extended.
		if(row > 1)
			table = realloc((void *)table,sizeof(char *) * (row + 1));
		
		int rowsize = 0;

		//Read the input char by char.
		while(cur != '\n' && cur != EOF){
			//Skip commas
			if(cur == ',')
				cur = getc(input);
			//For the first row, retreive the I/O names.
			else if(row == 0){
				int stringSize = 0;
				char * IOname = malloc(sizeof(char));

				while(cur != ',' && cur != '\n'){
					if(stringSize > 0)
						IOname = realloc((void *)IOname,sizeof(char)*(stringSize+1));
					IOname[stringSize] = cur;
					stringSize++;
					cur = getc(input);
				}
					IOname = realloc((void *)IOname,sizeof(char)*(stringSize+1));
					IOname[stringSize] = '\0';
					if(names > 0)
						IOnames = realloc((void *)IOnames,sizeof(char)*(names+1));
					IOnames[names] = IOname;
					names++;
			}
			//Throw the rest of the characters in an array.
			else{
				rowsize++;
				if (rowsize > rowmax)
					rowmax = rowsize;
				if (rowsize == 1)
					table[row] = malloc(sizeof(char));
				else
					table[row] = realloc((void *)table[row],sizeof(char) * rowsize);

				table[row][rowsize-1] = cur;

				cur = getc(input);
			}
		}

		if (cur == '\n')
			cur = getc(input);
		row++;

	}

	int i, j;
	int inCount = log10(row-1)/log10(2);
	char * inputs[inCount];
	for(i = 0; i < inCount; i++){
		inputs[i] = IOnames[i];
	}

	//Calculate and print results for each output.
	char *result;
	for(j = 0; j < rowmax - inCount; j++){
		int outs[row];
		for(i = 0; i < row-1; i++){
			if(table[i+1][inCount+j] == 48)
				outs[i] = 0;
			else
				outs[i] = 1;
		}
		result = solveTruthTable(&outs[0], inCount);
		printf("%s = %s\n",IOnames[inCount+j],result);
		//freePrime();
	}

	return 0; 
}
