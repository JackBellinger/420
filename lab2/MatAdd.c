#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi 
#include "../420-lab2/matlib.h" //parallel matrix library

int main(int argc, char** argv){
	
	MPI_Init(NULL, NULL);
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	srand(time(0));
	
	int print = atoi(argv[1]);
	int randmax = atoi(argv[2]);
	int rows = atoi(argv[3]);
	int cols = atoi(argv[4]);
	
	struct mat_ A, B, C;
	mat_init(&A, rows, cols, randmax);
	mat_init(&B, rows, cols, randmax);
	mat_init(&C, rows, cols, randmax);
	mat_add(&A, &B, &C);
	if(rank == 0 && print == 1){
		printf("Matrix A:\n");
		mat_print(&A);
		printf("\n");
		printf("Matrix B:\n");
		mat_print(&B);
		printf("\n");
		printf("Matrix A+B:\n");
		mat_print(&C);
	}
	
	free(A.arr);
	free(B.arr);
	free(C.arr);

	MPI_Finalize();
	return 0;
}