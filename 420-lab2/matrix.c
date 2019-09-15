#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define INDEX(n,m,i,j) m*i + j
#define ACCESS(A,i,j) A->arr[INDEX(A->rows, A->cols, i, j)]

typedef struct matrix{
	int rows, cols;
	int* arr;
} matrix;

void initMatrix(matrix* A, int r, int c){
	A->rows = r;
	A->cols = c;
	A->arr = malloc(r*c*sizeof(int));

	int i,j;
	for(i=0; i<r; i++)
		for(j=0; j<c; j++)
			ACCESS(A,i,j) = rand() % 100 + 1;
}

void printMatrix(matrix* A){
	int i,j;
	for(i=0; i<A->rows; i++){
		for(j=0; j<A->cols; j++){
			printf("%d ", ACCESS(A,i,j));
		}
		//printf("\n");
		puts("");
	}
}

void addMatrix
