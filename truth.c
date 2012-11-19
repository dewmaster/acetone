/*
	Presently a hot mess. Lots of commented out code/blocks. Needs all of the refactoring.
	
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "sMath.h"
int iVars = 3; //Count of input variables
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
	//newVal[i] = -1;
	//newVal = (int *)realloc((void *)newVal, i+1);
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
*/
void quine(int ** arr, int * ones, int size, int * diff){
	int * newOnes = malloc(sizeof(int *) * 2*size);
	int **newPrimes = multiDim(2*size,pow(2,size));
	
	int newDiff[size];
	int status[size];
	int count = 0;
	int i,j,k,l;
	//printf("onessss: %d\n",ones[0]);
	//Set newDiff and status to 0 for all values.
	for(i = 0; i < size; i++){
		newDiff[i] = 0;
		status[i] = 0;
	}
	
	for (i = 0; i < size; i++){
		int found = 0;
		//printf("i = %d\n",i);
		
		for(j = 0; j < size; j++){
			
			//Determine if all of the n+1 values have a difference of n+1.
			int correctDiff = 1;
			int lastDiff = -500;
			//printf("ones: %d    %d\n",ones[i],ones[j]);
			
			if (ones[i] + 1 == ones[j]){
				//printf("ones: %d    %d\n",ones[i+1],ones[j+1]);
				
				//Iterate through values in each set and make sure that the difference is the same and a power of 2.
				k = 0;
				l = 0;
				while(arr[j][k] != -1){
					if((log2(arr[j][l]-arr[i][k]) - (int)log2(arr[j][l]-arr[i][k])) != 0){correctDiff--;}
					if(arr[j][l]-arr[i][k] != lastDiff && lastDiff != -500) {correctDiff--;}
					lastDiff = (arr[j][l]-arr[i][k]);
					//printf("Diffff: %d\n",(arr[j][l]-arr[i][k]));
					l++;
					k++;
				}
					
			}
			
			//printf("Cdiff %d\n",correctDiff);
			//If a value is in the set n+1, and the difference is a power of 2.
			if(correctDiff == 1 && lastDiff != -500){
				//printf("Diff = %d\n",lastDiff);
				
				k = 0;
				l = 0;
				while(arr[i][k] != -1){
					//printf("count %d\n",count);
					newPrimes[count][l] = arr[i][k];
					k++;
					l++;
					//printf("Add to newPrimes\n");
				}
				k=0;
				newOnes[count] = ones[i];
				//printf("Add to newPrimes\n");
				while(arr[j][k] != -1){
					newPrimes[count][l] = arr[j][k];
					k++;
					l++;
				}
				newPrimes[count][l] = -1;
				newDiff[count] = diff[i] + lastDiff;
				//printf("i: %d, k: %d",i,k);
				status[i] = 1;
				status[j] = 1;
				count++;
				found = 1;
			}
		}
		//If the set doesn't pair with any values from within the set n+1, then it is
		//an esential prime implicant.
		//printf("Status: %d\n",status[i]);
		if (found == 0 && status[i] != 1){
			addToPrimes(arr[i],diff[i]);
			status[i] = -1;
		}
	}
	for(i = count; i < 2*size; i++){
			//if(status[i] != 1) {
				free((void *)newPrimes[i]);
				//free((void *)newOnes[i]);
			//}
		}
	newPrimes = realloc((void *)newPrimes,sizeof(int **)*count);
	newOnes = realloc((void *)newOnes,sizeof(int *)*count);
	
	//printf("NewPrimes: \n");
	//printMultiArray(newPrimes, count);
	//printf("\n");
	if(count > 0){
		quine(newPrimes, newOnes, count, &newDiff[0]);
		for(i = 0; i < count; i++){
			//if(status[i] != 1) {
				free((void *)newPrimes[i]);
				//free((void *)newOnes[i]);
			//}
		}
	}
	free((void *)newPrimes);
	

	free((void *)newOnes);
	//for(i = 0; i < count; i++){
	//	if(status[i] == 1) free((void *)newPrimes[i]);
	//}
	//free((void *)newPrimes);
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
	Uses the essential primes found by the quine function to find a 
	minimal solution to the truth table.
*/
void mccluskey(int size, int * imps){
	int ** grid = multiDim(primeSize,size);
	int ** ePrimes = malloc(sizeof(int *) * size);
	int ePrimeList[size];
	if (ePrimes == NULL) exit(1);
	int ePrimeCount = 0;
	//printf("size = %d\n",size);
	int i,j,k,l,a;
	
	//Fill grid
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
	//printf("Finished filling\n");
	//printMultiArray(grid,primeSize);
	
	/*
		Cycle through the grid matrix checking for new "Essential Primes" and eliminating their columns until there are none left to be found.
	*/
	int newEPrimes = 1;
	while (newEPrimes != 0){
		
		newEPrimes = 0;
		//Find Essential primes
		for(i = 0; i < size; i++){
			//printf("Go #%d\n",i);
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
				
				//ePrimes[ePrimeCount] = malloc(sizeof(int)*10);
				ePrimes[ePrimeCount] = copyOf(prime[lastPrime]);
				ePrimeList[ePrimeCount] = lastPrime;
				//printf("yup\n");
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
	//printf("Rows Counted.\n");
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
	//printf("Cols Counted.\n");
	cols = realloc((void *)cols,sizeof(int) * cCount);
	//printf("Row number  is %d\n", rCount);
	//printf("Col number  is %d\n", cCount);
	/*
		Determine all k-combinations of the remaining primes, increasing k until full coverage is reached.
	*/
	int ** demPrimes = malloc(sizeof(int *) * rCount);
	//int * demPrimes = malloc(sizeof(int) * rCount);
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

	//Parse answer and print result;
	printf("The result is:\n");

	for(i = 0; i < ePrimeCount; i++){
		int * bin = convertToBin(ePrimes[i][0], iVars);

		int * one = convertToBin(primeOnes[ePrimeList[i]], iVars);
		//printf("Ones : %d\n", primeOnes[ePrimeList[i]]);
		for(j = 0; j < iVars; j++){
			//printf("    %d",one[j]);
			if(one[j] != 1){
				if(bin[j] == 0){
					printf("~");
				}
				printf("%c",(char)(65+j));
			}
		}

		if(i != ePrimeCount - 1){
			printf(" + ");
		}
		free(one);
		free(bin);
	}
	if(minSize != 9999){
		for(i = 0; i < minSize; i++){
			int primeSame = -1;
			int * bin = convertToBin(minSolution[i][0], iVars);

			for( j = 0; j < primeSize; j++){
				if(pCompare(prime[j], minSolution[i]) == 1) {
					primeSame = j;
					break;
				}
			}
			//printf("Ones : %d\n", primeSame);
			int * one = convertToBin(primeOnes[primeSame], iVars);

			printf(" + ");

			for(j = 0; j < iVars; j++){
				if(one[j] != 1){
					if(bin[j] == 0){
						printf("~");
					}
					printf("%c",(char)(65+j));
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
	//printf("rCount is %d\n",rCount);
	if (rCount > 1) free(poss);
	printf("\n");
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
	//free((void *)ePrimes);
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
	//printf("Not Sort arrays\n");
}

void solveTruthTable(int * output, int size){
	iVars = size;
	int i,j;		//Loop indicies
	int impCount = 0; //Prime implicant countOn
	int * imps = malloc(sizeof(int) * pow(2,size));
	int ** input = generateTruthTable(size);
	//
	int **inArray=malloc(sizeof(int *) * pow(2,size));
	if (inArray == NULL) exit(1);
	for(i = 0; i < pow(2,size); i++){
			inArray[i] = malloc(sizeof(int) * 2);
			inArray[i][1] = -1;
		}
	
	int *inArrayOnes=(int *)malloc(sizeof(int) * pow(2,size));
	if (inArrayOnes == NULL) exit(1);
	
	//Loop through all output values to find prime implicants
	for(i = 0; i < pow(2,size); i++){
		if (output[i] == 1){
			int bitVal = 0; //The base 10 value of the input.
			int bitCount = 0;
			
			//Determine base 10 value for bits
			for(j = 0; j < size; j++){
				bitVal = bitVal + input[j][i]*(int)pow(2,j);
				if (input[j][i] == 1) bitCount++;
			}
			
			inArray[impCount][0] = bitVal;
			//printf("bcount: %d\n",);
			inArrayOnes[impCount] = bitCount;
			imps[impCount] = bitVal;
			impCount++;		//Update counter
		}
	}
	
	if(impCount == pow(2,iVars)){
		printf("The Result is: 1\n");
		for(i = 0; i <  pow(2,size); i++){
			free(inArray[i]);
		}
		free(inArray);
		
	}
	else if (impCount == 0){
		printf("The Result is: 0\n");
		for(i = 0; i <  pow(2,size); i++){
			free(inArray[i]);
		}
		free(inArray);
		
	}
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
		
		quine(inArray,inArrayOnes,impCount, &diff[0]);
		
		primeSortAndCompare();
		for(i = 0; i < primeSize; i++){
			for(j = 0; j < 10; j++){
				if (prime[i][j] ==-1){
					break;
				}
				//printf("%d, ",prime[i][j]);
			}
			//printf("     %d\n",primeOnes[i]);
		}
		
		mccluskey(impCount,imps);
		printf("Good\n");
		for (i = 0; i < impCount; i++){
			free(inArray[i]);
		}
		free(inArray);
		
	}

	for(i = 0; i < size; i++){
		free(input[i]);
	}
	free(input);
	free(inArrayOnes);
	free(imps);

}

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

int main(){
	
	//Test Case 1
	//Result Should be ~A~B + AB + ~AC
	int daFlip[] = {1,1,0,1,0,0,1,1};
	solveTruthTable(&daFlip[0], 3);
	freePrime();

	//Test Case 2
	//Result Should be ABC
	int daFlip1[] = {0,0,0,0,0,0,0,1};
	solveTruthTable(&daFlip1[0], 3);
	freePrime();

	//Test Case 3
	//Result Should be ~AB + BC
	int daFlip2[] = {0,0,1,1,0,0,0,1};
	solveTruthTable(&daFlip2[0], 3);
	freePrime();
	
	//Test Case 4
	//Result Should be ABC
	int daFlip3[] = {0,0,0,0,0,0,0,0};
	solveTruthTable(&daFlip3[0], 3);
	freePrime();

	//Test Case 5
	//Result Should be ABC
	int daFlip4[] = {1,1,1,1,1,1,1,1};
	solveTruthTable(&daFlip4[0], 3);
	freePrime();

	//Test Case 6
	//Result Should be ABC
	int daFlip5[] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	solveTruthTable(&daFlip5[0], 4);
	freePrime();

	//Test Case 7
	//Result Should be ABC
	int daFlip6[] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	solveTruthTable(&daFlip6[0], 5);
	freePrime();

	return 0; 
}
