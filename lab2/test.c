#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff
#include "matlib.h" //parallel matrix library

int main(int argc, char** argv){
	MPI_Init(NULL, NULL);

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);

	struct Matrix A, B, C;
	mat_init(&A, 3, 3, 10);
	mat_init(&B, 3, 3, 10);
	printf("Matrix A:\n");
	mat_print(&A);
	mat_inplace_transpose(&A);
	if(rank == 0){
		printf("Matrix A:\n");
		mat_print(&A);
		printf("\n");
		printf("Matrix B:\n");
		mat_print(&B);
		printf("\n");
		printf("Matrix A*B:\n");
		mat_print(&C);
	}

	MPI_Finalize();
	return 0;
}
