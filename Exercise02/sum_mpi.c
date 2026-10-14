// Exercise 2: add up the numbers 1 to 10,000,000 with MPI.
// Each process adds its own range of numbers; the workers send their partial sums to
// rank 0 with MPI_Send, and rank 0 receives them one by one with MPI_Recv.
#include <mpi.h>
#include <stdio.h>

#define N 10000000LL

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);          // start the clock together
    double start = MPI_Wtime();

    // This process adds the numbers first .. last (the last process takes any remainder)
    long long chunk = N / size;
    long long first = rank * chunk + 1;
    long long last = (rank == size - 1) ? N : first + chunk - 1;
    long long local_sum = 0;
    for (long long i = first; i <= last; i++)
        local_sum += i;

    if (rank != 0) {
        MPI_Send(&local_sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long long total = local_sum;
        for (int src = 1; src < size; src++) {
            long long part;
            MPI_Recv(&part, 1, MPI_LONG_LONG, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += part;
        }
        double elapsed = MPI_Wtime() - start;
        long long expected = N * (N + 1) / 2;
        printf("processes = %d, sum = %lld, expected = %lld, correct = %s, time = %.6f s\n",
               size, total, expected, total == expected ? "YES" : "NO", elapsed);
    }

    MPI_Finalize();
    return 0;
}
