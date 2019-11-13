#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi 
#include <math.h>
#include "../420-lab6/matlib.h" //parallel matrix library

int main(int argc, char** argv){
	
	MPI_Init(NULL, NULL);
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	srand(time(0));
	
	int n = 4;
	
	struct Matrix A, B, C;
	mat_init(&A, 1, n, 1);
	mat_init(&B, n, n, 10);
	mat_init(&C, 1, n, 10);
	
	if(rank == 0){
		printf("Vector:\n");
		mat_print(&A);
		printf("Matrix:\n");
		mat_print(&B);
	}
	
	float old = 1.0, curr = 0.0;
	while(fabs(old-curr) > 0.001){
		old = curr;
		mat_multiply(&A, &B, &C);
		curr = normalize(&C);
		printf("normies REE %f, node %d\n",curr,rank);
		mat_equals(&A, &C);
	}
	
	if(rank == 0){
		printf("Eigenvector:\n");
		mat_print(&A);
		mat_print(&B);
		mat_print(&C);
	}
	mat_multiply(&A, &B, &C);
	if(rank == 0){
		printf("Eigenvalue: %f\n", norm2(&C)/norm2(&A));
	}
	
	free(A.arr);
	free(B.arr);
	free(C.arr);

	MPI_Finalize();
	return 0;
}