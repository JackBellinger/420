#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff


int main(int argc, char** argv){
	MPI_Init(NULL, NULL);

	// int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
	//                   MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
	// void *sendbuf //input
	// void *recvbuf
	// int count //how many things are in the bufs / how much data
	// MPI_Datatype datatype //datatype
	// MPI_Op op
	// MPI_Comm comm
	MPI_Comm world = MPI_COMM_WORLD;
	int nprocs, me;

	MPI_Comm_size(world, &nprocs);
	MPI_Comm_rank(world, &me);
	srand(time(0) + me);
	float my_number, sum;
	my_number = (float)rand() / (float) RAND_MAX;

	printf("Rank %d 's number is %2.4f\n",me, my_number);

	MPI_Allreduce(&my_number, &sum, 1, MPI_FLOAT, MPI_SUM, world);
	float avg = sum / nprocs;
	if(me == 0)
		printf("the average is %2.4f\n", avg);

	MPI_Finalize();
	return 0;
}
