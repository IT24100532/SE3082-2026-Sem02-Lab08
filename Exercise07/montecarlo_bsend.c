// Exercise 7: Exercise 6 rewritten with Buffered Send (MPI_Bsend).
// Each process throws its share of random points (with its own seed); the workers send
// their "inside" counts to rank 0, which accepts them from ANY sender, in arrival order.
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define POINTS 10000000LL

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    long long my_points = POINTS / size + (rank < POINTS % size ? 1 : 0);
    unsigned int seed = 12345 + 1000 * rank;
    long long inside = 0;
    for (long long i = 0; i < my_points; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0)
            inside++;
    }

    if (rank != 0) {
        // Bsend copies the message into a buffer we attach, then returns at once,
        // without waiting for rank 0 to post its receive.
        int bufsize;
        MPI_Pack_size(1, MPI_LONG_LONG, MPI_COMM_WORLD, &bufsize);
        bufsize += MPI_BSEND_OVERHEAD;
        void *buffer = malloc(bufsize);
        MPI_Buffer_attach(buffer, bufsize);
        MPI_Bsend(&inside, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
        MPI_Buffer_detach(&buffer, &bufsize);   // waits until the buffered message has left
        free(buffer);
    } else {
        long long total = inside;
        printf("received from:");
        for (int src = 1; src < size; src++) {
            long long part;
            MPI_Status status;
            MPI_Recv(&part, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            printf(" %d", status.MPI_SOURCE);   /* who actually sent this one */
            total += part;
        }
        double elapsed = MPI_Wtime() - start;
        printf("\nprocesses = %d, pi = %.6f, inside = %lld, time = %.6f s\n",
               size, 4.0 * total / POINTS, total, elapsed);
    }

    MPI_Finalize();
    return 0;
}
