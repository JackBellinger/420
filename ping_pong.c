#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff


int main(int argc, char** argv){
	if( argc < 2)
	{
		puts("Usage: ./bin N\n");
		return 1;
	}

	MPI_Init(NULL, NULL);

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);

	int token = 0;
	int ping_pong_limit = atoi(argv[1]);

	while( token < ping_pong_limit)
	{
		if( token % 2 == rank % 2 )
		{
			token++;
			MPI_Send(&token, 1, MPI_INT, (rank + 1) % 2, 0, world);
			printf("Proc %d sent %d to %d\n", rank, token, (rank + 1) % 2);
		}else
		{
			MPI_Recv(&token, 1, MPI_INT, (rank + 1) % 2, 0, world, MPI_STATUS_IGNORE);
			printf("Proc %d recieved %d from %d\n", rank, token, (rank + 1) % 2);
		}
	}
	MPI_Finalize();
	return 0;
}
