#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff

void print_array(int A[], int size)
{
		int i;
		for( i = 0; i < size; i++)
			printf("%d, ", A[i]);
		printf("\n");
}
int main(int argc, char** argv){
	MPI_Init(NULL, NULL);

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);

	int array_size = atoi(argv[1]);

	int arr1 [array_size];
	int arr2 [array_size];
	srand(time(0));

	int i;
	int inner_product_check = 0;
	for( i = 0; i < array_size; i ++ )
	{
		arr1[i] = rand() % 10 + 1;
		arr2[i] = rand() % 10 + 1;
		inner_product_check += arr1[i] * arr2[i];
	}
 	if(rank == 0)
	{
		printf("Unchunked arrays\n");
		print_array(arr1, array_size);
		print_array(arr2, array_size);
	 	printf("inner product check: %d\n", inner_product_check);
 	}
	int chunk_size = array_size / (world_size);
	int leftovers = (array_size % world_size) + chunk_size;

	int send_counts[world_size];
	int displs[world_size];

	for( i = 0; i < world_size; i++ )
	{
		displs[i] = i * (chunk_size);
		if( i == world_size - 1)
			send_counts[i] = leftovers;
		else
			send_counts[i] = chunk_size;
	}
	// printf("Send array: \n");
	// print_array(send_counts, world_size);

	int recieve_array1[send_counts[rank]];
	for( i = 0; i < send_counts[rank]; i++ )
		recieve_array1[i] = 0;

	int recieve_array2[send_counts[rank]];
	for( i = 0; i < send_counts[rank]; i++ )
		recieve_array2[i] = 0;

	MPI_Scatterv(
		arr1,
		send_counts,
		displs,
		MPI_INT,
		recieve_array1,
		send_counts[rank],
		MPI_INT,
		0,
		world
	);

	MPI_Scatterv(
		arr2,
		send_counts,
		displs,
		MPI_INT,
		recieve_array2,
		send_counts[rank],
		MPI_INT,
		0,
		world
	);

	// printf("chunked array at node %d\n", rank);
	// print_array(recieve_array1, send_counts[rank]);
	// print_array(recieve_array2, send_counts[rank]);

	int pip = 0;
	for( i = 0; i < send_counts[rank]; i++)
		pip += recieve_array1[i] * recieve_array2[i];
	//printf("partial inner product at node %d = %d\n", rank, pip);

	int fip = 0;
	MPI_Reduce(
		&pip,
		&fip,
		1,
		MPI_INT,
		MPI_SUM,
		0,
		world
	);
	if(rank == 0)
		printf("Inner Product: %d\n", fip);
	MPI_Finalize();
	return 0;
}
