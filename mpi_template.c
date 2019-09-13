#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff


int main(int argc, char** argv){
	MPI_Init(NULL, NULL);
	
	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(&rank, world);
	MPI_Comm_size(&world_size, world);



	MPI_Finalize();
	return 0;
}
