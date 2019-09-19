#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi
#include "matlib.h" //parallel matrix library

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	srand(time(0));

	int print = 0, randmax = 100, rows1 = 2, cols1rows2 = 0, cols2 = 0;
	switch(argc)
	{
		case 6:
			print = atoi(argv[5]);
		case 5:
			randmax = atoi(argv[4]);
		case 4:
			cols2 = atoi(argv[3]);
		case 3:
			cols1rows2 = atoi(argv[2]);
		case 2:
			rows1 = atoi(argv[1]);
	}
	if( !cols1rows2 )
		cols1rows2 = rows1;
	if( !cols2 )
		cols2 = rows1;

	struct Matrix A, B, C;
	mat_init(&A, rows1, cols1rows2, randmax);
	mat_init(&B, cols1rows2, cols2, randmax);
	mat_init(&C, rows1, cols2, randmax);
	mat_multiply(&A, &B, &C);
	if(rank == 0 && print == 1){
		printf("Matrix A:\n");
		mat_print(&A);
		printf("\n");
		printf("Matrix B:\n");
		mat_print(&B);
		printf("\n");
		printf("Matrix A*B:\n");
		mat_print(&C);
	}

	free(A.arr);
	free(B.arr);
	free(C.arr);

	MPI_Finalize();
	return 0;
}
