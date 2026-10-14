// Exercise 5.2: message2.cc rewritten with Buffered Send (MPI_Bsend).
// Each value gets its own variable (numbers[0..2]) instead of reusing one variable,
// and the buffer is attached once, before the sends.
// mpicxx message2_bsend.cc -o message2_bsend && mpirun -np 2 ./message2_bsend
#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int COUNT = 3;
    int numbers[COUNT];         // one variable per message, never overwritten

    if (rank == 0) {
        // Buffer big enough for 3 messages of one int each, plus MPI's overhead per message
        int packsize;
        MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &packsize);
        int bufsize = COUNT * (packsize + MPI_BSEND_OVERHEAD);
        void* buffer = std::malloc(bufsize);
        MPI_Buffer_attach(buffer, bufsize);

        for (int i = 0; i < COUNT; i++) {
            numbers[i] = i * 10;
            MPI_Bsend(&numbers[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << numbers[i] << std::endl;
        }

        MPI_Buffer_detach(&buffer, &bufsize);   // returns once all buffered messages have left
        std::free(buffer);
    } else if (rank == 1) {
        for (int i = 0; i < COUNT; i++) {
            MPI_Recv(&numbers[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << numbers[i] << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
