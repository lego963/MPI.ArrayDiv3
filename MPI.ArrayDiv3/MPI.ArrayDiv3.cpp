// MPI.ArrayDiv3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <Windows.h>
#include <iostream>

using namespace std;

// size of array 
const int LENGTH = 12000;

using namespace std;

void init_array(int arr[])
{
	srand(GetTickCount64());
	for (int i = 0; i < LENGTH; i++)
	{
		arr[i] = rand() % 100;
	}
}

int sum_nonpp_func(int arr[]) {
	int local = 0;
	for (size_t i = 0; i < LENGTH; i++)
	{
		if (arr[i] % 3 == 0)
		{
			local += arr[i];
		}
	}
	return local;
}


int main(int argc, char* argv[])
{
	int array_of_elems[LENGTH], total_sum = 0, th_sum = 0;
	init_array(array_of_elems);

	int sum_nopp_var = sum_nonpp_func(array_of_elems);
	cout << "SUM OF ARRAY DIV 3 NOT PP: " << sum_nopp_var << endl;

	int th_num, th_rank;
	unsigned int const min_block_size = 1000;
	unsigned int const max_threads = (LENGTH + min_block_size - 1) / min_block_size;

	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &th_num);
	th_num = min(th_num != 0 ? th_num : 2, max_threads);
	MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);

	MPI_Bcast(array_of_elems, LENGTH, MPI_INT, 0, MPI_COMM_WORLD);

	int chunk_size = LENGTH / th_num;
	int left_ind = chunk_size * th_rank;
	int right_ind = chunk_size * (th_rank + 1);

	if (th_rank == th_num - 1) {
		right_ind = LENGTH;
	}

	for (int i = left_ind; i < right_ind; i++)
	{
		if (array_of_elems[i] % 3 == 0)
		{
			th_sum += array_of_elems[i];
		}
	}

	if (th_rank == 0)
	{
		total_sum = th_sum;
		for (int i = 1; i < th_num; i++)
		{
			MPI_Recv(&th_sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
			total_sum = total_sum + th_sum;
		}
	}
	else
	{
		MPI_Send(&th_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	if (th_rank == 0) {
		cout << "SUM OF ARRAY DIV 3 PP: " << total_sum << endl;
	}

	MPI_Finalize();
	return 0;
}
