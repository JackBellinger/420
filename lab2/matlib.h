#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff

#define INDEX(n,m,i,j) m*i + j
#define ACCESS(A,i,j) A->arr[INDEX(A->rows, A->cols, i, j)]

typedef struct mat_{
	int rows, cols;
	int* arr;
} mat_;

void mat_init(mat_* A, int r, int c, int max_rand){
	A->rows = r;
	A->cols = c;
	A->arr = malloc(r*c*sizeof(int));

	int i,j;
	for(i=0; i<r; i++)
		for(j=0; j<c; j++)
			ACCESS(A,i,j) = rand() % max_rand + 1;
}

void mat_print(mat_* A){
	int i,j;
	for(i=0; i<A->rows; i++){
		for(j=0; j<A->cols; j++){
			printf("%d ", ACCESS(A,i,j));
		}
		//printf("\n");
		puts("");
	}
}

void mat_add(mat_* A, mat_* B, mat_* result)
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
			A->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		MPI_Scatterv(
			B->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array2,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		for( i = 0; i < send_counts[rank]; i++)
			recieve_array1[i] += recieve_array2[i];

		MPI_Gatherv(
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			result->arr,
			send_counts,
			displs,
			MPI_INT,
			0,
			world
		);

	}

}//end mat_add

void mat_subtract(mat_* A, mat_* B, mat_* result)
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
			A->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		MPI_Scatterv(
			B->arr,
			send_counts,
			displs,
			MPI_INT,
			recieve_array2,
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		for( i = 0; i < send_counts[rank]; i++)
			recieve_array1[i] -= recieve_array2[i];

		MPI_Gatherv(
			recieve_array1,
			send_counts[rank],
			MPI_INT,
			result->arr,
			send_counts,
			displs,
			MPI_INT,
			0,
			world
		);

	}

}//end mat_subtract

void mat_multiply (mat_* A, mat_* B, mat_* C){
	
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);
	
	if(A->cols == B->rows && A->rows == C->rows && B->cols == C->cols){
		int i,j,k,m;
		int row[A->cols];
		int col[A->cols];
		for(m = 0; m < C->rows; m++){
			for(j = 0; j < C->cols; j++){
				for(k = 0; k < A->cols; k++){
					row[k] = A->arr[(m*A->cols)+k];
					col[k] = B->arr[j+(k*B->cols)];
				}
				int array_size = A->cols;
				int chunk_size = array_size / (world_size);
				int leftovers = (array_size % world_size) + chunk_size;
				int send_counts[world_size];
				int displs[world_size];
				
				for(i = 0; i < world_size; i++){
					displs[i] = i * (chunk_size);
					if( i == world_size - 1)
						send_counts[i] = leftovers;
					else
						send_counts[i] = chunk_size;
				}
				int recieve_array1[send_counts[rank]];
				for(i = 0; i < send_counts[rank]; i++ )
					recieve_array1[i] = 0;
				
				int recieve_array2[send_counts[rank]];
				for(i = 0; i < send_counts[rank]; i++ )
					recieve_array2[i] = 0;
				
				MPI_Scatterv(
					row,
					send_counts,
					displs,
					MPI_INT,
					recieve_array1,
					send_counts[rank],
					MPI_INT,
					0,
					world
				);

				MPI_Scatterv(
					col,
					send_counts,
					displs,
					MPI_INT,
					recieve_array2,
					send_counts[rank],
					MPI_INT,
					0,
					world
				);
				
				int pip = 0;
				for( i = 0; i < send_counts[rank]; i++)
					pip += recieve_array1[i] * recieve_array2[i];
				
				int fip = 0;
				MPI_Reduce(
					&pip,
					&fip,
					1,
					MPI_INT,
					MPI_SUM,
					0,
					world
				);
				if(rank == 0)
					ACCESS(C,m,j) = fip;
			}
		}
	}else{
		if(rank == 0)
			printf("Matrix Multiplication is not valid on this set of matrices.\n");
	}
}//end mat_multiply

void mat_transpose(mat_* A, mat_* B){
	if(A->rows == B->cols && A->cols == B->rows){
		int i, j;
		for(i = 0; i < A->cols; i++){
			for(j = 0; j < A->rows; j++){
				ACCESS(B,i,j) = ACCESS(A,j,i);
			}
		}
	}else
		printf("Matrix Transpose is not valid on this set of matricies.\n");
}//end mat_transpose