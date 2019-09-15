#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff

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
			ACCESS(A,i,j) = 1;//rand() % 100 + 1;
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
void print_array(int A[], int size)
{
		int i;
		for( i = 0; i < size; i++)
			printf("%d, ", A[i]);
		printf("\n");
}
void addMatrix(matrix* A, matrix* B, matrix* result)
{


}
int main(){
	MPI_Init(NULL, NULL);
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	if( rank == 0 )
	{
		srand(time(0));

		struct matrix A;
		initMatrix(&A, 4, 4);
		printMatrix(&A);

		struct matrix B;
		initMatrix(&B, 4, 4);
		printMatrix(&B);

		struct matrix C;
		initMatrix(&C, 4, 4);
		addMatrix(&A, &B, &C);

		printf("Result:\n");
		printMatrix(&C);

		free(A.arr);
		free(B.arr);
		free(C.arr);
	}
	MPI_Finalize();
	return 0;
}
