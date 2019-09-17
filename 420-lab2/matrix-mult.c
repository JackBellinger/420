#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <mpi.h>

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
      ACCESS(A,i,j) = i * r + j;//rand() % 100 + 1;
}

void printMatrix(matrix* A){
  int i,j;
  for(i=0; i<A->rows; i++){
    for(j=0; j<A->cols; j++){
      printf("%d ", ACCESS(A,i,j));
    }
    puts("");
  }
}

void multipyMatrix (matrix* A, matrix* B, matrix* C){

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);


	if(A->cols == B->rows && A->rows == B->cols && A->rows == C->rows && B->cols == C->cols){
		int i,j,k;
		int row[A->rows];
		int col[A->rows];
		for(i = 0; i < C->rows; i++){
			for(j = 0; j < C->cols; j++){
				for(k = 0; k < A->rows; k++){
					row[k] = A->arr[(i*A->cols)+k];
					col[k] = B->arr[j+(k*A->cols)];
				}
				int array_size = A->rows;
				int arr1 [array_size];
				int arr2 [array_size];
				int chunk_size = array_size / (world_size);
				int leftovers = (array_size % world_size) + chunk_size;
				int send_counts[world_size];
				int displs[world_size];

				for( i = 0; i < world_size; i++ ){
					displs[i] = i * (chunk_size);
					if( i == world_size - 1)
						send_counts[i] = leftovers;
					else
						send_counts[i] = chunk_size;
				}
				int recieve_array1[send_counts[rank]];
				for( i = 0; i < send_counts[rank]; i++ )
					recieve_array1[i] = 0;
					int recieve_array2[send_counts[rank]];
				for( i = 0; i < send_counts[rank]; i++ )
					recieve_array2[i] = 0;
				MPI_Scatterv(
					&arr1,
					send_counts,
					displs,
					MPI_INT,
					&recieve_array1,
					send_counts[rank],
					MPI_INT,
					0,
					world
				);

				MPI_Scatterv(
					&arr2,
					send_counts,
					displs,
					MPI_INT,
					&recieve_array2,
					send_counts[rank],
					MPI_INT,
					0,
					world
				);
				int pip = 0;
				for( i = 0; i < send_counts[rank]; i++)
					pip += recieve_array1[i] * recieve_array2[i];

				int fip = 0;
				//printf("Before Reduce\n");
				MPI_Reduce(
					&pip,
					&fip,
					1,
					MPI_INT,
					MPI_SUM,
					0,
					world
				);
				//printf("After Reduce\n");


				if(rank == 0)
					ACCESS(C,i,j) = fip;
			}
		}
	}else{
		printf("Matrix Multiplication is not valid on this set of matrices.\n");
	}
}

void transposeMatrix (matrix* A){
	int i,j,swap;
	for(i = 1; i < A->cols; i++){
		for(j = 0; j < i; j++){
			swap = ACCESS(A,i,j);
			ACCESS(A,i,j) = ACCESS(A,j,i);
			ACCESS(A,j,i) = swap;
		}
	}
}

int main(int argc, char** argv){
	MPI_Init(NULL, NULL);

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);

	srand(time(0));


	struct matrix A;
	initMatrix(&A, 3, 3);
	if(rank == 0)
	{
		printMatrix(&A);
		printf("\n");
	}
	struct matrix B;
	initMatrix(&B, 3, 3);
	if(rank == 0)
	{
		printMatrix(&B);
		printf("\n");
	}

	struct matrix C;
	initMatrix(&C, 3, 3);

	multipyMatrix(&A, &B, &C);
	if(rank == 0)
	{
		printf("Multiplication result:\n");
		printMatrix(&C);
	}

	transposeMatrix(&A);
	if(rank == 0)
	{
		printf("Transposed matrix A\n");
		printMatrix(&A);
	}

	free(A.arr);
	free(B.arr);
	free(C.arr);

	MPI_Finalize();
	return 0;
}
