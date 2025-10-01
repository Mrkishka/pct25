#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  double start_time, end_time;
  start_time = MPI_Wtime();

  size_t size = 10;
  char *rbuf = new char[size * num_procs + 1];
  std::sprintf(rbuf + size * rank, "Message %d\n", rank);

  int right = (rank + 1) % num_procs, left = (rank - 1 + num_procs) % num_procs;

  for (int i = 0; i < num_procs - 1; ++i)
  {
    int send_block = (rank - i + num_procs) % num_procs,
        recv_block = (rank - i - 1 + num_procs) % num_procs;
    MPI_Sendrecv(rbuf + send_block * size, size, MPI_CHAR, right, 0,
                 rbuf + recv_block * size, size, MPI_CHAR, left, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  std::ofstream outfile("ring_output.txt", std::ios::app);
  outfile << "Process " << rank << " has received\n"
          << rbuf << '\n';
  outfile.close();

  end_time = MPI_Wtime();

  if (rank == 0)
  {
    std::ofstream timefile("ring_time.txt", std::ios::app);
    timefile << "Time for " << num_procs << " processes: "
             << (end_time - start_time) * 1000 << " ms\n";
    timefile.close();
  }

  delete[] rbuf;

  MPI_Finalize();
}