//int arrayOfElements[LENGTH];
//init_array(arrayOfElements);
//
//int count_nopp_var = count_nonpp(arrayOfElements);
//cout << "SUM OF ARRAY DIV 3 NOT PP: " << count_nopp_var << endl;
//
//int proc_num = 0;
//
//int pid, np,
//elements_per_process,
//n_elements_recieved;
//// np -> no. of processes 
//// pid -> process id 
//
//MPI_Status status;
//
//unsigned int const minBlockSize = 1000;
//unsigned int const max_threads = (LENGTH + minBlockSize - 1) / minBlockSize;
//proc_num = min(max_threads != 0 ? max_threads : 2, max_threads);
//
//// Creation of parallel processes 
//MPI_Init(&argc, &argv);
//
//// find out process ID, 
//// and how many processes were started 
//MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
//MPI_Comm_rank(MPI_COMM_WORLD, &pid);
//
//
//// master process 
//if (pid == 0) {
//	int index, i;
//	elements_per_process = LENGTH / proc_num;
//
//
//	// check if more than 1 processes are run 
//	if (proc_num > 1) {
//		// distributes the portion of array 
//		// to child processes to calculate 
//		// their partial sums 
//		for (i = 1; i < proc_num - 1; i++) {
//			index = i * elements_per_process;
//
//			MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
//			MPI_Send(&arrayOfElements[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
//		}
//
//		// last process adds remaining elements 
//		index = i * elements_per_process;
//		int elementsLeft = LENGTH - index;
//
//		MPI_Send(&elementsLeft, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
//		MPI_Send(&arrayOfElements[index], elementsLeft, MPI_INT, i, 0, MPI_COMM_WORLD);
//	}
//
//	// master process add its own sub array 
//	int sum = 0;
//	for (i = 0; i < elements_per_process; i++)
//		if (arrayOfElements[i] % 3 == 0) {
//			//cout << "ELEMENT FROM MASTER: " << arrayOfElements[i] << endl;
//			sum += arrayOfElements[i];
//		}
//
//	// collects partial sums from other processes 
//	int tmp;
//	for (i = 1; i < proc_num; i++) {
//		MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
//		int sender = status.MPI_SOURCE;
//
//		sum += tmp;
//	}
//
//	// prints the final sum of array 
//	printf("SUM OF ARRAY DIV 3 PP : %d\n", sum);
//}
//else {	// slave processes 
//	MPI_Recv(&n_elements_recieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//
//	// stores the received array segment 
//	// in local array a2 
//	MPI_Recv(&slave_array, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//
//	// calculates its partial sum 
//	int partial_sum = 0;
//	for (int i = 0; i < n_elements_recieved; i++)
//		if (slave_array[i] % 3 == 0) {
//			//cout << "ELEMENT FROM THREAD " << pid << ": " << arrayOfElements[i] << endl;
//			partial_sum += slave_array[i];
//		}
//	// sends the partial sum to the root process 
//	MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//}
//
//// cleans up all MPI state before exit of process 
//MPI_Finalize();
//
//return 0;