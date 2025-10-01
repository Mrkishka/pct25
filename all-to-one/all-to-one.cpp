#include <mpi.h>
#include <cstring>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  int rank, num_procs, root = 0;
  double start_time, end_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  start_time = MPI_Wtime();

  size_t size = 10;

  char sbuf[size];
  std::sprintf(sbuf, "Message %d\n", rank);
  char *rbuf = (rank == root) ? new char[size * num_procs] : new char[size];

  if (rank == root)
  {
    std::memcpy(rbuf, sbuf, size);
    for (int p_num = 1; p_num < num_procs; p_num++)
    {
      MPI_Recv(rbuf + p_num * size, size, MPI_CHAR, p_num, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
    }

    std::ofstream outfile("all-to-one_output.txt", std::ios::app);
    outfile << "Process " << rank << " received message\n"
            << rbuf << "from all other processes\n";
    outfile.close();
  }
  else
  {
    MPI_Send(sbuf, size, MPI_CHAR, root, 0, MPI_COMM_WORLD);
  }

  end_time = MPI_Wtime();

  if (rank == root)
  {
    std::ofstream timefile("all-to-one_time.txt", std::ios::app);
    timefile << "Time for " << num_procs << " processes: "
             << (end_time - start_time) * 1000 << " ms\n";
    timefile.close();
  }

  delete[] rbuf;

  MPI_Finalize();
}