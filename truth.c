#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "sMath.h"
int **prime;
int *primeOnes;
int primeSize = 0;


int * copyOf(int * arr){
	int i = 0;
	int * newVal = malloc(sizeof(int) * 1);
	//newVal[0] = 0;
	newVal[0] = -1; 
	i = 0;
	while(arr[i] != -1){
		if(newVal[i] == -1){
			newVal = realloc((void *)newVal,sizeof(int)*(i+2));
			newVal[i+1] = -1;
		}
		newVal[i] = arr[i];
		i++;
	}
	return newVal;
}

/*
	Print a multi-dimensional array.
*/
void printMultiArray(int ** arr, int size){
	int i,j;
	for(i = 0; i < size; i++){
		j = 0;
		while(arr[i][j] != -1){
			printf("%d, ",arr[i][j]);
			j++;
		}
		printf("\n");
	}
}

/*
	Sort primes by length, then numbers.

	Global Dep - prime & primeSize.
*/
void primeSortAndCompare(){
	int i,j,k,l,swap;
	int pLength[primeSize];

	//Sort bits in a set
	for(i = 0; i < primeSize; i++){
		j = 0; k = 0; l = 0;
		while(prime[i][k] != -1){
		pLength[i] = k+1;
			l = 0;
			while(prime[i][l+1] != -1){
				
				if(prime[i][l] > prime[i][l+1]){
					swap = prime[i][l];
					prime[i][l] = prime[i][l+1];
					prime[i][l+1] = swap;
				}
				l++;
			}
			k++;
		}
	}

	//Remove duplicate sets
	for(i = 0; i < primeSize-1; i++){
		for(j = i+1; j < primeSize; j++){
			int same = 1;
			k = 0;
			while(prime[i][k] != -1 && prime[j][k] != -1){
				if(prime[i][k] != prime[j][k]){
					same = 0;
					break; 
				}
				k++;
			}
			
			if(same == 1){
				int * swap = prime[j];
				prime[j] = prime[primeSize - 1];
				prime[primeSize - 1] = swap;
				primeSize--;
			}
			
		}
	}
	
	//Sort Sets
	for(i = 0; i < primeSize; i++){
		for(j = 0; j < primeSize-1; j++){
			if(pLength[j] > pLength[j+1]){
				int * swap = prime[j+1];
				prime[j+1] = prime[j];
				prime[j] = swap;
				int swapL = pLength[j+1];
				pLength[j+1] = pLength[j];
				pLength[j] = swapL;
			}
			
			if(pLength[j] == pLength[j+1]){
				k = 0;
				while(prime[j][k] != -1){
					if(prime[j][k] > prime[j+1][k]){
						int * swap = prime[j+1];
						prime[j+1] = prime[j];
						prime[j] = swap;
						int swapL = pLength[j+1];
						pLength[j+1] = pLength[j];
						pLength[j] = swapL;
					}
					k++;
				}
			}
			
		}
	}
}

/*
	Add new array to the array of prime implicants.

	Global Dep - prime, primeSize, & primeOnes.
*/
void addToPrimes(int *arr, int num){
	
	int i, j, count;
	if (primeSize == 0){
		primeSize++;
		prime = malloc(sizeof(int *));
		primeOnes = malloc(sizeof(int));
	}
	else{
		primeSize++;
		prime = realloc((void *)prime,sizeof(int *)*primeSize);
		primeOnes = realloc((void *)primeOnes,sizeof(int)*primeSize);
	}	
	int newValsize = 6;
	primeOnes[primeSize-1] = num;
	prime[primeSize-1] = copyOf(arr);;
}

/*
	Create a multidimensional array with size1 rows and size2 columns.
*/
int ** multiDim(int size1, int size2){
	int i;
	int **arr=malloc(sizeof(int **) * size1);
	if (arr == NULL) exit(1);
	
	for(i = 0; i < size1; i++){
			arr[i] = malloc(sizeof(int *) * size2+1);
			if (arr[i] == NULL) exit(1);
			arr[i][size2] = -1;
	}
	return arr;
}

/*
	Remove an element el from an array arr.
*/
int * rmElement(int * arr, int * size, int el){
	int i, j;
	for( i = el; i < *size - 1; i++){
		int swap = arr[i];
		arr[i] = arr[i+1];
		arr[i+1] = swap;
	}
	*size = *size -1;
	return realloc((void *)arr,sizeof(int)*(*size));
}

/*
	Sort two arrays.
*/
void sortDualArrays(int ** arr1,int * arr2, int size){
	int i,j,swap1,swap2;
	for (i = 0; i< size; i++){
		for(j = 0; j < size - 1; j++){
			if(arr2[j] > arr2[j+1]){
				swap1 = arr1[j][0];
				swap2 = arr2[j];
				arr1[j][0] = arr1[j+1][0];
				arr2[j] = arr2[j+1];
				arr1[j+1][0] = swap1;
				arr2[j+1] = swap2;
			} 
		}
	}
}

/*
	Compare to arrays to determine if their contents are the same.
*/
int pCompare(int * a, int * b){
	int i, j, k;
	i = 0;
	while(a[i] != -1){
		//if( b[i] == -1) return 0;
		if( b[i] != a[i]) return 0;
		i++;
	}
	i=0;
	while(b[i] != -1){
		//if( a[i] == -1) return 0;
		if( a[i] != b[i]) return 0;
		i++;
	}
	return 1;
}

/*
	Remove column col from and array by zeroing out all of the values.
	
	This makes it possible to remove the essential prime implicants from the grid.
*/
void rmColumn(int ** arr, int rows, int col){
	int i,j,k;
	for(i = 0; i < rows; i++){
		j = col;
		arr[i][col] = 0;
	}
}

/*
	Finds all of the essential primes in the truth table.

	Global Dep - Indirect through addToPrimes.
*/
void findEssentialPrimes(int ** arr, int * ones, int size, int * diff){
	int * newOnes = malloc(sizeof(int *) * 2*size);
	int **newPrimes = multiDim(2*size,pow(2,size));
	int newDiff[size];
	int status[size];
	int count = 0;
	int i,j,k,l;

	//Set newDiff and status to 0 for all values.
	for(i = 0; i < size; i++){
		newDiff[i] = 0;
		status[i] = 0;
	}
	


	for (i = 0; i < size; i++){
		int found = 0;
		
		for(j = 0; j < size; j++){
			
			//Determine if all of the n+1 values have a difference of n+1.
			int correctDiff = 1;
			int lastDiff = -500;
			
			if (ones[i] + 1 == ones[j]){
				
				//Iterate through values in each set and make sure that the difference is the same and a power of 2.
				k = 0;
				l = 0;
				while(arr[j][k] != -1){
					if((log2(arr[j][l]-arr[i][k]) - (int)log2(arr[j][l]-arr[i][k])) != 0){correctDiff--;}
					if(arr[j][l]-arr[i][k] != lastDiff && lastDiff != -500) {correctDiff--;}
					lastDiff = (arr[j][l]-arr[i][k]);
					l++;
					k++;
				}
					
			}
			
			//If a value is in the set n+1, and the difference is a power of 2.
			if(correctDiff == 1 && lastDiff != -500){
				
				k = 0;
				l = 0;
				while(arr[i][k] != -1){
					newPrimes[count][l] = arr[i][k];
					k++;
					l++;
				}
				k=0;
				newOnes[count] = ones[i];
				while(arr[j][k] != -1){
					newPrimes[count][l] = arr[j][k];
					k++;
					l++;
				}
				newPrimes[count][l] = -1;
				newDiff[count] = diff[i] + lastDiff;
				status[i] = 1;
				status[j] = 1;
				count++;
				found = 1;
			}
		}
		//If the set doesn't pair with any values from within the set n+1, then it is
		//an esential prime implicant.
		if (found == 0 && status[i] != 1){
			addToPrimes(arr[i],diff[i]);
			status[i] = -1;
		}
	}
	for(i = count; i < 2*size; i++){
			free((void *)newPrimes[i]);
		}
	newPrimes = realloc((void *)newPrimes,sizeof(int **)*count);
	newOnes = realloc((void *)newOnes,sizeof(int *)*count);
	
	if(count > 0){
		findEssentialPrimes(newPrimes, newOnes, count, &newDiff[0]);
		for(i = 0; i < count; i++){
			free((void *)newPrimes[i]);
		}
	}
	free((void *)newPrimes);
	free((void *)newOnes);
}

/*
	Adds a character to the end of a string.
*/
char * addToString(char * string, int * size, char new){
	*size = *size + 1;
	if(*size == 1){
		string = malloc(sizeof(char)*2);
		string[*size-1] = new;
		string[*size] = 0;
	}
	else{
		string = realloc((void *)string,sizeof(char)*(*size+1));
		string[*size-1] = new;
		string[*size] = 0;
	}
	return string;
}

/*
	Uses the essential primes found by the findEssentialPrimes function to find a 
	minimal solution to the truth table.
*/
char * mccluskey(int size, int * imps, int inputCount){
	int ** grid = multiDim(primeSize,size);
	int ** ePrimes = malloc(sizeof(int *) * size);
	int ePrimeList[size];
	if (ePrimes == NULL) exit(1);
	int ePrimeCount = 0;
	int i,j,k,l,a;
	char *output;
	int outputSize = 0;
	
	//Fill grid with 0s.
	for(i = 0; i < primeSize; i++){
		for(j = 0; j < size; j++){
			grid[i][j] = 0;
			k = 0;
			while(prime[i][k] != -1){
				if(prime[i][k] == imps[j]){
					grid[i][j] = 1;
				}
				k++;
			}
		}
	}	
	
	/*
		Cycle through the grid matrix checking for new "Essential Primes" and eliminating their columns until there are none left to be found.
	*/
	int newEPrimes = 1;
	while (newEPrimes != 0){
		
		newEPrimes = 0;
		//Find Essential primes
		for(i = 0; i < size; i++){
			int check = 0;
			int lastPrime;
			for(j = 0; j < primeSize; j++){
				if(grid[j][i] == 1){
					check++;
					lastPrime = j;
				}
			}
			for(k = 0; k < ePrimeCount; k++){
				if(ePrimeList[k] == lastPrime) check = 0;
			}
			if(check == 1){
				
				ePrimes[ePrimeCount] = copyOf(prime[lastPrime]);
				ePrimeList[ePrimeCount] = lastPrime;
				ePrimeCount++;
				newEPrimes++;
			}
		}
		
		//Eliminate columns covered by essential primes
		for(i = 0; i < ePrimeCount; i++){
			k = 0;
			while(ePrimes[i][k] != -1){
				for(j = 0; j < size; j++){
					if (imps[j] == ePrimes[i][k]){
						rmColumn(grid, primeSize, j);
					}
				}
				k++;
			}
		}
	}
	
	/*
		Generate minimum cover with remaining primes
	*/
	
	//	Find the number that contain 1's and store their indicies for easy access.
	int * rows = malloc(sizeof(int) * (primeSize-ePrimeCount));
	int * cols = malloc(sizeof(int) * primeSize);
	int rCount = 0;
	int cCount = 0;
	for(i = 0; i < primeSize; i++){
		j = 0;
		while(grid[i][j] != -1){
			if(grid[i][j] == 1){
				rows[rCount] = i;
				rCount++;
				break;
			}
			j++;
		}
	}
	j = 0;
	while(grid[0][j] != -1){
		for(i = 0; i < primeSize; i++){
			if(grid[i][j] == 1){
				cols[cCount] = imps[i];
				cCount++;
				break;
			}
		}
		j++;
	}
	cols = realloc((void *)cols,sizeof(int) * cCount);
	 
	/*
		Determine all k-combinations of the remaining primes, increasing k until full coverage is reached.
	*/
	int ** demPrimes = malloc(sizeof(int *) * rCount);
	for( i = 0; i < rCount; i++){
		demPrimes[i] = copyOf(prime[rows[i]]);
	} 

	//Determine if full coverage is met.
	int minSize = 9999;
	int ** minSolution;
	int *** poss;
	for( l = 1; l < rCount; l++){
		poss = combinations(demPrimes, rCount, l);

		
		for( i = 0; i< combo(rCount, l); i++){
			//Copy cols
			int copyCount = cCount;
			int * colsC = malloc(sizeof(int) * cCount);
			for( k = 0; k < cCount; k++){
				colsC[k] = cols[k];
			}

			for( j = 0; j< l; j++){
				k=0;
				while(poss[i][j][k] != -1){
					for(a = 0; a < copyCount; a++){
						if(colsC[a] == poss[i][j][k]) colsC = rmElement(colsC, &copyCount, a);
					}
					k++;
				}
			}
			if(copyCount == 0){
				if(minSize > l){
					minSize = l;
					minSolution = poss[i];
					break;
				}
			}

			free((void *)colsC);
		}
		if(minSize != 9999) break;
		else{
			for(i = 0; i < combo(rCount, l); i++){
				for( j = 0; j < l; j++){
					free(poss[i][j]);
				}
				free(poss[i]);
			}
		}
	}

	//Parse answer and print result
	for(i = 0; i < ePrimeCount; i++){
		int * bin = convertToBin(ePrimes[i][0], inputCount);

		int * one = convertToBin(primeOnes[ePrimeList[i]], inputCount);
		for(j = 0; j < inputCount; j++){
			if(one[j] != 1){
				if(bin[j] == 0){
					output = addToString(output,&outputSize,'~');

				}
				output = addToString(output,&outputSize,(char)(65+j));
			}
		}

		if(i != ePrimeCount - 1){
			output = addToString(output,&outputSize,' ');
			output = addToString(output,&outputSize,'+');
			output = addToString(output,&outputSize,' ');
		}
		free(one);
		free(bin);
	}
	if(minSize != 9999){
		for(i = 0; i < minSize; i++){
			int primeSame = -1;
			int * bin = convertToBin(minSolution[i][0], inputCount);

			for( j = 0; j < primeSize; j++){
				if(pCompare(prime[j], minSolution[i]) == 1) {
					primeSame = j;
					break;
				}
			}
			int * one = convertToBin(primeOnes[primeSame], inputCount);
			output = addToString(output,&outputSize,' ');
			output = addToString(output,&outputSize,'+');
			output = addToString(output,&outputSize,' ');

			for(j = 0; j < inputCount; j++){
				if(one[j] != 1){
					if(bin[j] == 0){
						output = addToString(output,&outputSize,'~');
					}
					output = addToString(output,&outputSize,(char)(65+j));
				}
			}
			free(bin);
			free(one);
		}

		for(i = 0; i < combo(rCount, minSize); i++){
			for( j = 0; j < minSize; j++){
				free(poss[i][j]);
			}
			free(poss[i]);
		}
	}
	if (rCount > 1) free(poss);
	free((void *)demPrimes);
	free((void *)rows);
	free((void *)cols);
	for(i = 0;i < ePrimeCount; i++){
		free((void *)ePrimes[i]);
	}
	free((void *)ePrimes);

	for(i = 0;i < primeSize; i++){
		free((void *)grid[i]);
	}
	free((void *)grid);

	return output;
}

char * solveTruthTable(int * output, int size){
	int i,j;		//Loop indicies
	int impCount = 0; //Prime implicant countOn
	int * imps = malloc(sizeof(int) * pow(2,size));
	int ** input = generateTruthTable(size);
	int **inArray=malloc(sizeof(int *) * pow(2,size));
	char * out;
	int outputSize = 0;
	if (inArray == NULL) exit(1);
	for(i = 0; i < pow(2,size); i++){
			inArray[i] = malloc(sizeof(int) * 2);
			inArray[i][1] = -1;
		}
	
	int *inArrayOnes=(int *)malloc(sizeof(int) * pow(2,size));
	if (inArrayOnes == NULL) exit(1);
	
	//Loop through all output values and make a list of the 'ones'.
	for(i = 0; i < pow(2,size); i++){
		if (output[i] == 1){
			int bitVal = 0; //The base 10 value of the input.
			int bitCount = 0;
			
			//Determine base 10 value for bits
			for(j = 0; j < size; j++){
				bitVal = bitVal + input[j][i]*(int)pow(2,j);
				if (input[j][i] == 1) bitCount++;
			}
			
			//Save the value.
			inArray[impCount][0] = bitVal;
			inArrayOnes[impCount] = bitCount;
			imps[impCount] = bitVal;
			impCount++;		//Update counter
		}
	}
	

	//If every output = 1.
	if(impCount == pow(2,size)){
		out = addToString(out,&outputSize,'1');
		outputSize = 1;
		for(i = 0; i <  pow(2,size); i++){
			free(inArray[i]);
		}
		free(inArray);
		
	}
	//If every output = 0.
	else if (impCount == 0){
		out = addToString(out,&outputSize,'0');
		outputSize = 1;
		for(i = 0; i <  pow(2,size); i++){
			free(inArray[i]);
		}
		free(inArray);
	}
	//Calculate the other outputs.
	else{
		for(i = impCount; i < pow(2,size); i++){
			free((void *)inArray[i]);
		}

		inArray = realloc((void *)inArray,sizeof(int *) * impCount);
		inArrayOnes = (int *)realloc((void *)inArrayOnes,sizeof(int) * impCount);
		imps = (int *)realloc((void *)imps,sizeof(int)*impCount);
		
		sortDualArrays(inArray,inArrayOnes,impCount);
		
		int diff[impCount];
		for(i = 0; i < impCount; i++){
			diff[i] = 0;
		}
		
		findEssentialPrimes(inArray,inArrayOnes,impCount, &diff[0]);
		
		primeSortAndCompare();
		for(i = 0; i < primeSize; i++){
			for(j = 0; j < 10; j++){
				if (prime[i][j] ==-1){
					break;
				}
			}
		}
		
		out = mccluskey(impCount,imps,size);

		for (i = 0; i < impCount; i++){
			free(inArray[i]);
		}
		free(inArray);
		
	}


	//Free everything left.
	for(i = 0; i < size; i++){
		free(input[i]);
	}
	free(input);
	free(inArrayOnes);
	free(imps);
	
	//Free primes.
	if(primeSize > 0){
		for( i = 0; i< primeSize; i++){
			free((void *)prime[i]);
		}
		free((void *)prime);
		free((void *)primeOnes);
	}
	primeSize = 0;

	return out;
}

/*
	Frees memory used by the prime array.
*/
void freePrime(){
	int i;
	if(primeSize > 0){
		for( i = 0; i< primeSize; i++){
			free((void *)prime[i]);
		}
		free((void *)prime);
		free((void *)primeOnes);
	}
	primeSize = 0;
}

void parseCSV(){
	
}

int main(int argc, char *argv[]){

	FILE * input;

	input = fopen(argv[1],"r");
	char cur = getc(input);
	int row = 0;
	char ** table = malloc(sizeof(char *));
	int rowmax = 0;

	while(cur != EOF){
		if(row > 0)
			table = realloc((void *)table,sizeof(char *) * (row + 1));
		int rowsize = 0;

		while(cur != '\n'){

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

		if (cur == '\n')
			cur = getc(input);
		row++;

	}

	char * value;
	int outs[row+1];
	int i;
	for(i = 0; i < row; i++){
		if(table[i][rowmax-1] == 48)
			outs[i] = 0;
		else
			outs[i] = 1;
	}

	value = solveTruthTable(&outs[0], rowmax-1);
	printf("%s\n",value);
	freePrime();
	free(value);
	/*
	char * value;
	//Test Case 1
	//Result Should be ~A~B + AB + ~AC
	int daFlip[] = {1,1,0,1,0,0,1,1};
	value = solveTruthTable(&daFlip[0], 3);
	printf("%s\n",value);
	freePrime();
	free(value);

	//Test Case 2
	//Result Should be ABC
	int daFlip1[] = {0,0,0,0,0,0,0,1};
	value = solveTruthTable(&daFlip1[0], 3);
	printf("%s\n",value);
	freePrime();
	free(value);

	//Test Case 3
	//Result Should be ~AB + BC
	int daFlip2[] = {0,0,1,1,0,0,0,1};
	value = solveTruthTable(&daFlip2[0], 3);
	printf("%s\n",value);
	freePrime();
	free(value);

	//Test Case 4
	//Result Should be 0
	int daFlip3[] = {0,0,0,0,0,0,0,0};
	value = solveTruthTable(&daFlip3[0], 3);
	printf("%s\n",value);
	freePrime();
	free(value);

	//Test Case 5
	//Result Should be 1
	int daFlip4[] = {1,1,1,1,1,1,1,1};
	value = solveTruthTable(&daFlip4[0], 3);
	printf("%s\n",value);
	freePrime();
	free(value);

	//Test Case 6
	//Result Should be ~A~B~C~D + ~AB~CD + A~BC~D + ABCD
	int daFlip5[] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	value = solveTruthTable(&daFlip5[0], 4);
	printf("%s\n",value);
	freePrime();
	free(value);

	//Test Case 7
	//Result Should be ~B~C~D~E + ~BC~DE + B~CD~E + BCDE
	int daFlip6[] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	value = solveTruthTable(&daFlip6[0], 5);
	printf("%s\n",value);
	freePrime();
	free(value);
	*/

	return 0; 
}
