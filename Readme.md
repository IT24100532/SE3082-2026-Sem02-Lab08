## <div align="center">Lab 08 – MPI</div>

SE3082 – Parallel Computing, Semester 2, 2026

> The lab sheet ([LabSheet08-MPI.pdf](LabSheet08-MPI.pdf)) is an older copy titled
> "Lab Sheet 6 – SE4060, Semester 2, 2021". It is the Lab 08 sheet for SE3082.

| Folder | Exercise |
|---|---|
| `Exercise00/` | In-class activities from [SE4060-MPI-Part1](https://github.com/SLIIT-FacultyOfComputing/SE4060-MPI-Part1): `HelloMPI.c`, `messages1.cc`, `messaage2.cc` |
| (this repo) | Exercise 1: GitHub repo for the solutions |
| `Exercise02/` | Sum of 1 to 10,000,000 with MPI |
| `Exercise03/` | Monte Carlo Pi with MPI (10,000,000 points) |
| `Exercise04/` | Time and speedup graphs for Exercises 2 and 3 |
| `Exercise05/` | Source/destination mismatch, and `message2.cc` with `MPI_Bsend` |
| `Exercise06/` | Exercise 3 with `MPI_ANY_SOURCE` |
| `Exercise07/` | Exercise 6 with `MPI_Bsend` |
| `samples/` | `message1.cc` and `message2.cc` from the lab |

Build everything with `make` and run all exercises with `make run` (needs `mpicc`/`mpicxx`).
Each folder has a `.txt` file with the output and the answers.
