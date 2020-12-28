// MPI.ArrayDiv3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <Windows.h>


// size of array 
const int LENGTH = 10000;

// Temporary array for slave process 
int a2[1000];

void init_array(int arr[])
{
	srand(GetTickCount64());
	for (int i = 0; i < LENGTH; i++)
		arr[i] = rand() % 100;
}


int main(int argc, char* argv[])
{
	int arrayOfElements[LENGTH];
	init_array(arrayOfElements);

	int pid, np,
		elements_per_process,
		n_elements_recieved;
	// np -> no. of processes 
	// pid -> process id 

	MPI_Status status;

	// Creation of parallel processes 
	MPI_Init(&argc, &argv);

	// find out process ID, 
	// and how many processes were started 
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);


	// master process 
	if (pid == 0) {
		int index, i;
		elements_per_process = LENGTH / np;
		unsigned int const minBlockSize = 1000;
		unsigned int const maxThreads = (LENGTH + minBlockSize - 1) / minBlockSize;

		// check if more than 1 processes are run 
		if (np > 1) {
			// distributes the portion of array 
			// to child processes to calculate 
			// their partial sums 
			for (i = 1; i < np - 1; i++) {
				index = i * elements_per_process;

				MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&arrayOfElements[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
			}

			// last process adds remaining elements 
			index = i * elements_per_process;
			int elementsLeft = LENGTH - index;

			MPI_Send(&elementsLeft, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&arrayOfElements[index], elementsLeft, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		// master process add its own sub array 
		int sum = 0;
		for (i = 0; i < elements_per_process; i++)
			sum += arrayOfElements[i];

		// collects partial sums from other processes 
		int tmp;
		for (i = 1; i < np; i++) {
			MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			int sender = status.MPI_SOURCE;

			sum += tmp;
		}

		// prints the final sum of array 
		printf("Sum of array is : %d\n", sum);
	}
	else {	// slave processes 
		MPI_Recv(&n_elements_recieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		// stores the received array segment 
		// in local array a2 
		MPI_Recv(&a2, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		// calculates its partial sum 
		int partial_sum = 0;
		for (int i = 0; i < n_elements_recieved; i++)
			partial_sum += a2[i];

		// sends the partial sum to the root process 
		MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	// cleans up all MPI state before exit of process 
	MPI_Finalize();

	return 0;
}
