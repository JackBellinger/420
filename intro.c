#include <stdio.h>
#include <mpi.h>
int main(int argc, char ** argv)
{
	MPI_Init(&argc, &argv);
	int world_size;
	MPI_Comm world =  MPI_COMM_WORLD;
	MPI_Comm_size(world, &world_size);
	int me;
	MPI_Comm_rank(world, &me);
	
	char name [MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(name, &name_len);

	printf("Hello World (size is %d) \n", world_size);
	printf("I am processor %s", name);
	printf("I am node %d\n", me);
	
	
	MPI_Finalize();
	return 0;
}
