#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int num_nodes;
	MPI_Comm world =  MPI_COMM_WORLD;
	MPI_Comm_size(world, &num_nodes);
	int node_num;
	MPI_Comm_rank(world, &node_num);
	
	unsigned long n = atoi(argv[1]);
	int upperbound = sqrt(n);
	int found_divisor = 0;
	int i;
	for(i = 2; i <= upperbound; i++)
	{
		if(i % num_nodes == node_num)
		{
			if(n % i == 0)
			{
				found_divisor = 1;
				printf("%d is a divisor of %d\n", i, n);
				//mpi send
				break;
			}
		}
	}

	MPI_Finalize();
	//mpi recieve	
	if(found_divisor == 0)
	{
		printf("%d is a prime\n", n);
	}

	return 0;
}
