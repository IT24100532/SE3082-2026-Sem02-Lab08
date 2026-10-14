// Exercise 5.1: message1.cc changed so the source and destination do not match.
// Rank 0 still sends to rank 1, but rank 1 now waits for a message from rank 2.
// mpicxx mismatch.cc -o mismatch && mpirun -np 3 ./mismatch
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number;
    if (rank == 0) {
        number = 42;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent " << number << std::endl;
    } else if (rank == 1) {
        std::cout << "Process 1 waiting for a message from process 2..." << std::endl;
        MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // wrong source
        std::cout << "Process 1 received " << number << std::endl;
    }

    MPI_Finalize();
    return 0;
}
