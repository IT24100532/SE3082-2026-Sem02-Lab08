// Exercise 3: Pi with the Monte Carlo method and MPI, 10,000,000 points in total.
// Each process throws its share of random points (with its own seed); the workers send
// their "inside" counts to rank 0, which receives them from each source IN RANK ORDER.
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
        MPI_Send(&inside, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long long total = inside;
        printf("received from:");
        for (int src = 1; src < size; src++) {
            long long part;
            MPI_Recv(&part, 1, MPI_LONG_LONG, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf(" %d", src);
            total += part;
        }
        double elapsed = MPI_Wtime() - start;
        printf("\nprocesses = %d, pi = %.6f, inside = %lld, time = %.6f s\n",
               size, 4.0 * total / POINTS, total, elapsed);
    }

    MPI_Finalize();
    return 0;
}
