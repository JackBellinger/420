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
	if( A->rows != B->rows || A->cols != B->cols)
	{
		printf("Operation not possible with those matricies");
	}
	else
	{
		MPI_Comm world = MPI_COMM_WORLD;
		int rank, world_size;
		MPI_Comm_rank(world, &rank);
		MPI_Comm_size(world, &world_size);

		int array_size = A->rows * A->cols;
		int chunk_size = array_size / (world_size);
		int leftovers = (array_size % world_size) + chunk_size;

		int send_counts[world_size];
		int displs[world_size];

		int i;
		for( i = 0; i < world_size; i++ )
		{
			printf("chunking\n");
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

		printf("scattering\n");
		MPI_Scatterv(
			&(A->arr),
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
			&(B->arr),
			send_counts,
			displs,
			MPI_INT,
			&recieve_array2,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		// printf("chunked array at node %d\n", rank);
		// print_array(recieve_array1, send_counts[rank]);
		// print_array(recieve_array2, send_counts[rank]);

		for( i = 0; i < send_counts[rank]; i++)
			recieve_array1[i] += recieve_array2[i];

		printf("gathering\n");
		print_array(recieve_array1, send_counts[rank]);
		MPI_Gatherv(
			&recieve_array1,
			send_counts[rank],
			MPI_INT,
			&(result->arr),
			send_counts,
			displs,
			MPI_INT,
			0,
			world
		);

	}

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
