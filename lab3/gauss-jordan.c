#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <mpi.h> // mpi stuff
#inlcude "matlib.h"

int gauss_jordan()
{
	for(j=1; j<=n; j++)
	{
		for(i=1; i<=n; i++)
		{
			if(i!=j)
			{
				c=A[i][j]/A[j][j];
				for(k=1; k<=n+1; k++)
				{
					A[i][k]=A[i][k]-c*A[j][k];
				}
			}
		}
	}
}
int main(int argc, char** argv){
	MPI_Init(NULL, NULL);

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(&rank, world);
	MPI_Comm_size(&world_size, world);



	MPI_Finalize();
	return 0;
}
