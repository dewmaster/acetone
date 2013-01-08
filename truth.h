#ifndef TRUTH_H
#define TRUTH_H

int * copyOf(int * arr);
void printMultiArray(int ** arr, int size);
void primeSortAndCompare();
void addToPrimes(int *arr, int num);
int ** multiDim(int size1, int size2);
int * rmElement(int * arr, int * size, int el);
void sortDualArrays(int ** arr1,int * arr2, int size);
int pCompare(int * a, int * b);
void rmColumn(int ** arr, int rows, int col);
void findEssentialPrimes(int ** arr, int * ones, int size, int * diff);
char * addToString(char * string, int * size, char new);
char * mccluskey(int size, int * imps, int inputCount);
char * solveTruthTable(int * output, int size);
void freePrime();
void parseCSV();

#endif