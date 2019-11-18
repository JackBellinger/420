#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi
#include <math.h>
#include "matlib.h" //parallel matrix library

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	srand(time(0));

	if(argc != 2){
		puts("You need a matrix size");
		exit(1);
	}
	int n = atoi(argv[1]);

	struct Matrix A, temp, x;
	mat_init(&A, n, n, 10);
	mat_init(&x, n, 1, 1);
	mat_init(&temp, n, 1, 10);

	// if(rank == 0){
	// 	printf("Vector:\n");
	// 	mat_print(&x);
	// 	printf("Matrix:\n");
	// 	mat_print(&A);
	// }

	int cont = 1;
	double norm, tolerance = 0.001;
	//don't increase precision past this
	//we think that the scatters are blocking because the precision gets too high
	//for mpi to use non-blocking sends, and node 0 sends to itself
	//we plan to fix this in the updated matrix multiplication I'm working on
	while(cont){
		mat_multiply(&A, &x, &temp);
		norm = normalize(&temp);
		// if(rank == 0)
		// 	printf("norm is %f\n", norm);
		cont = t_check(&x, &temp, tolerance);
		mat_equals(&x, &temp);
	}

	// if(rank == 0){
	// 	printf("\nEigenvector:\n");
	// 	mat_print(&temp);
	// }
	mat_multiply(&A, &x, &temp);
	// if(rank == 0)
	// 	printf("\nEigenvalue: %f\n", norm2(&temp)/norm2(&x));

	free(A.arr);
	free(x.arr);
	free(temp.arr);

	MPI_Finalize();
	return 0;
}
