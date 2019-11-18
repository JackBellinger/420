#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff
#include "matlib.h"


int main(int argc, char** argv){
	MPI_Init(NULL, NULL);

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);

	int n = 4;

	struct Matrix A, B, C;
	mat_init(&A, n, n, 10);
	mat_init(&B, n, n, 10);
	mat_init(&C, n, n, 1);


	mat_chunk_multiply(&A, &B, &C);

	if(rank == 0){
		printf("A:\n");
		mat_print(&A);
		printf("\nB:\n");
		mat_print(&B);
		printf("\nC:\n");
		mat_print(&C);
	}
	free(A.arr);
	free(B.arr);
	free(C.arr);

	MPI_Finalize();
	return 0;
}
