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

void mat_init(mat_* A, int r, int c){
	A->rows = r;
	A->cols = c;
	A->arr = malloc(r*c*sizeof(int));

	int i,j;
	for(i=0; i<r; i++)
		for(j=0; j<c; j++)
			ACCESS(A,i,j) = i * r + j;//rand() % 100 + 1;
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
void print_array(int A[], int size)
{
		int i;
		for( i = 0; i < size; i++)
			printf("%d, ", A[i]);
		printf("\n");
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
		//send_counts[0] = 0;
		// printf("chunking\n");

		for( i = 0; i < world_size; i++ )
		{
			displs[i] = i * (chunk_size);
			if( i == world_size - 1)
				send_counts[i] = leftovers;
			else
				send_counts[i] = chunk_size;
		}
		// print_array(send_counts, world_size);
		int recieve_array1[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array1[i] = 0;

		int recieve_array2[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array2[i] = 0;

		// printf("scattering\n");
		MPI_Scatterv(
			A->arr,//no &?
			send_counts,
			displs,
			MPI_INT,
			recieve_array1,//no &?
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		MPI_Scatterv(
			B->arr,//no &?
			send_counts,
			displs,
			MPI_INT,
			recieve_array2,//no &?
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

		// printf("gathering\n");
		// print_array(recieve_array1, send_counts[rank]);
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
		//send_counts[0] = 0;
		// printf("chunking\n");

		for( i = 0; i < world_size; i++ )
		{
			displs[i] = i * (chunk_size);
			if( i == world_size - 1)
				send_counts[i] = leftovers;
			else
				send_counts[i] = chunk_size;
		}
		// print_array(send_counts, world_size);
		int recieve_array1[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array1[i] = 0;

		int recieve_array2[send_counts[rank]];
		for( i = 0; i < send_counts[rank]; i++ )
			recieve_array2[i] = 0;

		// printf("scattering\n");
		MPI_Scatterv(
			A->arr,//no &?
			send_counts,
			displs,
			MPI_INT,
			recieve_array1,//no &?
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		MPI_Scatterv(
			B->arr,//no &?
			send_counts,
			displs,
			MPI_INT,
			recieve_array2,//no &?
			send_counts[rank],
			MPI_INT,
			0,
			world
		);

		// printf("chunked array at node %d\n", rank);
		// print_array(recieve_array1, send_counts[rank]);
		// print_array(recieve_array2, send_counts[rank]);

		for( i = 0; i < send_counts[rank]; i++)
			recieve_array1[i] -= recieve_array2[i];

		// printf("gathering\n");
		// print_array(recieve_array1, send_counts[rank]);
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

void mat_multiply(mat_* A, mat_* B, mat_* C){


	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);


	if(A->cols == B->rows && A->rows == B->cols && A->rows == C->rows && B->cols == C->cols){
		int i,j,k,m;
		int row[A->rows];
		int col[A->rows];
		for(m = 0; m < C->rows; m++){
			//printf("Outer Loop:%d\n", rank);
			//printf("i = %d, C -> rows = %d\n", i, C->rows);
			for(j = 0; j < C->cols; j++){
				//printf("Inner Loop%d\n", rank);
				for(k = 0; k < A->rows; k++){
					row[k] = A->arr[(m*A->cols)+k];
					col[k] = B->arr[j+(k*A->cols)];
				}
				int array_size = A->rows;
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
				//printf("chunked arrays at node %d\n", rank);
				//print_array(recieve_array1, send_counts[rank]);
				//print_array(recieve_array2, send_counts[rank]);

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
				//if(rank == 0)
					//printf("fip = %d\n", fip);


				if(rank == 0)
					//printf("m = %d, j = %d, fip = %d\n", m, j, fip);
					ACCESS(C,m,j) = fip;
			}
		}
	}else{
		printf("Matrix Multiplication is not valid on this set of matrices.\n");
	}
}//end mat_multipy

void mat_transpose (mat_* A){
	int i,j,swap;
	for(i = 1; i < A->cols; i++){
		for(j = 0; j < i; j++){
			swap = ACCESS(A,i,j);
			ACCESS(A,i,j) = ACCESS(A,j,i);
			ACCESS(A,j,i) = swap;
		}
	}
}

int main(){
	MPI_Init(NULL, NULL);
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	srand(time(0));

	struct mat_ A;
	mat_init(&A, 4, 4);
	if( rank == 0 )
		mat_print(&A);

	struct mat_ B;
	mat_init(&B, 4, 4);
	if( rank == 0 )
		mat_print(&B);

	struct mat_ C;
	mat_init(&C, 4, 4);

	mat_add(&A, &B, &C);
	if( rank == 0 )
	{
		printf("Addition:\n");
		mat_print(&C);
	}

	mat_subtract(&A, &B, &C);
	if( rank == 0 )
	{
		printf("Subtraction:\n");
		mat_print(&C);
	}

	mat_multiply(&A, &B, &C);
	if( rank == 0 )
	{
		printf("Multiplication:\n");
		mat_print(&C);
	}
	mat_transpose(&C);
	if( rank == 0 )
	{
		printf("Transpose:\n");
		mat_print(&C);
	}
	free(A.arr);
	free(B.arr);
	free(C.arr);

	MPI_Finalize();
	return 0;
}
