# Lab 08 (MPI). Build: make     Run everything: make run
CC = mpicc
CXX = mpicxx
CFLAGS = -Wall -O2
PROGS = bin/HelloMPI bin/messages1 bin/messaage2 bin/message1 bin/message2 \
        bin/sum_mpi bin/montecarlo_mpi bin/mismatch bin/message2_bsend \
        bin/montecarlo_anysource bin/montecarlo_bsend

all: $(PROGS)

# Exercise 0 programs use C++ headers (<cstdio>), so they are built with mpicxx
bin/HelloMPI: Exercise00/HelloMPI.c | bin
	$(CXX) $(CFLAGS) -x c++ $< -o $@
bin/messages1: Exercise00/messages1.cc | bin
	$(CXX) $(CFLAGS) $< -o $@
bin/messaage2: Exercise00/messaage2.cc | bin
	$(CXX) $(CFLAGS) $< -o $@
bin/message1: samples/message1.cc | bin
	$(CXX) $(CFLAGS) $< -o $@
bin/message2: samples/message2.cc | bin
	$(CXX) $(CFLAGS) $< -o $@
bin/sum_mpi: Exercise02/sum_mpi.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/montecarlo_mpi: Exercise03/montecarlo_mpi.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/mismatch: Exercise05/mismatch.cc | bin
	$(CXX) $(CFLAGS) $< -o $@
bin/message2_bsend: Exercise05/message2_bsend.cc | bin
	$(CXX) $(CFLAGS) $< -o $@
bin/montecarlo_anysource: Exercise06/montecarlo_anysource.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/montecarlo_bsend: Exercise07/montecarlo_bsend.c | bin
	$(CC) $(CFLAGS) $< -o $@

bin:
	mkdir -p bin

run: all
	@echo "== Exercise 0"; mpirun -np 4 ./bin/HelloMPI; mpirun -np 4 ./bin/messages1; mpirun -np 4 ./bin/messaage2; echo
	@echo "== Sample programs"; mpirun -np 2 ./bin/message1; mpirun -np 2 ./bin/message2
	@echo "== Exercise 2"; for np in 1 2 4 8; do mpirun --oversubscribe -np $$np ./bin/sum_mpi; done
	@echo "== Exercise 3"; for np in 1 2 4 8; do mpirun --oversubscribe -np $$np ./bin/montecarlo_mpi; done
	@echo "== Exercise 5.2"; mpirun -np 2 ./bin/message2_bsend
	@echo "== Exercise 6"; for np in 1 2 4 8; do mpirun --oversubscribe -np $$np ./bin/montecarlo_anysource; done
	@echo "== Exercise 7"; for np in 1 2 4 8; do mpirun --oversubscribe -np $$np ./bin/montecarlo_bsend; done
	@echo "== Exercise 5.1 (hangs on purpose, stopped after 10 s)"; timeout 10 mpirun -np 3 ./bin/mismatch; echo "exit code $$? (124 = killed by timeout)"

clean:
	rm -rf bin
