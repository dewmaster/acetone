#include <stdlib.h>
#include <math.h>

/*
	Determine and return n!.
*/
int fact(int n){
	int i = n;
	int sum = 1;
	while(i>0){
		sum = sum*i;
		i--;
	}
	return sum;
}

/*
	Return the number of combinations of size k from a set of size n.
*/
int combo(int n, int k){
	int total = fact(n)/(fact(k) *fact(n-k));
	return total;
}

/*
	Determine all of the combinations of the array arr of size k.
*/
int *** combinations(int ** arr, int size, int k){
	int i,j;
	int index[k];
	for( i = 0; i < k; i++){
		index[i] = i; 
	}
	int resultCount = 0;
	int *** results = malloc(sizeof( int **) * combo(size, k));

	int done = 0;
	while (done == 0 && resultCount < combo(size, k)){

		//Allocate memory for combination of size k.
		results[resultCount] = malloc(sizeof(int *) * k);

		//Place the new values in the combination.
		for(i = 0; i < k; i++){
			results[resultCount][i] = arr[index[i]];
		}
		resultCount++;

		int carry = 0;
		for( i = k-1; i > -1; i--){
			//On the first iteration of the loop.
			if ( i == k - 1){
				if (index[i] < size) index[i] = index[i] + 1;
				if (index[i] == size){
					index[i] = index[i-1] + 1;
					carry = 1;
				}
			}
			else if(i != 0){
				if(carry){
					if(index[i] < size - k + i){
						index[i] = index[i] + 1;
						for( j = i; j < k - 1; j++){
							index[j+1] = index[j] + 1;
						}
						carry = 0;
					}
					if (index[i] == size - k + 1 + i){
						index[i] = index[i-1] + 1;
						carry = 1;
					}
				}
			}
			else{
				if(carry){
					if(index[i] < size - k ){
						index[i] = index[i] + 1;
						for( j = i; j < k - 1; j++){
							index[j+1] = index[j] + 1;
						}
						carry = 0;
					}
					if (index[0] == size - k + 1){
						done = 1;
						//printf("Done!\n");
					}
				}
			}
		}
	}
	return results;
}

/*
	Convert a number to a binary value with a fixed size.
*/
int * convertToBin(int num, int size){
	int i, j, max, count;
	max = 0;
	count = 0;
	while(num > pow(2,max)){
		max++;
	}
	int * result = malloc(sizeof(int) * size);
	for( i = size - 1; i > -1; i--){
		if(num - pow(2,i) >= 0){
			result[count] = 1;
			num = num - pow(2,i);
		}
		else{
			result[count] = 0;
		}
		count++;
	}
	return result;
}

int ** generateTruthTable(int vars){
	int i,j;
	int size = pow(2,vars);
	int ** table = malloc(sizeof(int *)*vars);


	for(i = 0; i < vars; i++){
		table[i] = malloc(sizeof(int)*size);
		int count = pow(2,i);
		int currInput = 0;
		for(j = 0; j < size; j++){
			if(count == 0 && currInput == 0){
				currInput = 1;
				count = pow(2,i);
			}
			else if(count == 0 && currInput == 1){
				currInput = 0;
				count = pow(2,i);
			}
			table[i][j] = currInput;
			count--;
		}
	}

	return table;
}