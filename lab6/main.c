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

	int n = 4;

	struct Matrix A, temp, x;
	mat_init(&A, n, n, 10);
	mat_init(&x, n, 1, 1);
	mat_init(&temp, n, 1, 10);

	if(rank == 0){
		printf("Vector:\n");
		mat_print(&x);
		printf("Matrix:\n");
		mat_print(&A);
	}

	int end = 0;
	float tolerance = 0.001;
	while(t_check(&x, &temp, tolerance)){
		mat_multiply(&A, &x, &temp);
		normalize(&temp);
		mat_equals(&x, &temp);
	}

	if(rank == 0){
		printf("Eigenvector:\n");
		mat_print(&A);
		mat_print(&x);
		mat_print(&temp);
	}
	mat_multiply(&A, &x, &temp);
	if(rank == 0){
		printf("Eigenvalue: %f\n", norm2(&temp)/norm2(&x));
	}

	free(A.arr);
	free(x.arr);
	free(temp.arr);

	MPI_Finalize();
	return 0;
}
