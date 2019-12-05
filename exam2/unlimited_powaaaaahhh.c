#include <stdlib.h> // rand
#include <stdio.h> // puts, //printf, etc
#include <time.h> // time
#include <mpi.h> // mpi
#include <math.h>
#include "matlib.h" //parallel matrix library

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	srand(1);

	int n = 8;

	struct Matrix A, temp, x;
	mat_init(&A, n, n, 0);
	mat_init(&x, n, 1, 1);
	mat_init(&temp, n, 1, 10);
	exam_matrix(&A);
	mat_print(&A);
	int cont = 1;
	double norm, tolerance = 0.001;//don't increase precision or the scatters block maybe somehow??
	while(cont){
		mat_multiply(&A, &x, &temp);
		norm = normalize(&temp);
		//if(rank == 0)
			//printf("norm is %f\n", norm);
		cont = t_check(&x, &temp, tolerance);
		mat_equals(&x, &temp);
	}

	if(rank == 0){
		printf("\nEigenvector:\n");
		mat_print(&temp);
	}
	mat_multiply(&A, &x, &temp);
	if(rank == 0)
		printf("\nEigenvalue: %f\n", norm2(&temp)/norm2(&x));

	free(A.arr);
	free(x.arr);
	free(temp.arr);

	MPI_Finalize();
	return 0;
}
