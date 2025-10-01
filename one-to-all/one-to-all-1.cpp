#include <mpi.h>
#include <cstring>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  double start_time, end_time;
  start_time = MPI_Wtime();

  int size = 12;
  MPI_Datatype datatype = MPI_CHAR;

  char *rbuf = new char[size];

  if (rank == 0)
  {
    for (int i = 1; i < num_procs; i++)
    {
      char sbuf[size];
      std::sprintf(sbuf, "Message %d", i);
      MPI_Send(sbuf, size, datatype, i, 0, MPI_COMM_WORLD);
    }
  }
  else
  {
    MPI_Recv(rbuf, size, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::ofstream outfile("one-to-all-1_output.txt", std::ios::app);
    outfile << "Process " << rank << " received message: " << rbuf
            << std::endl;
    outfile.close();
  }

  end_time = MPI_Wtime();

  if (rank == 0)
  {
    std::ofstream timefile("one-to-all-1_time.txt", std::ios::app);
    timefile << "Time for " << num_procs << " processes: "
             << (end_time - start_time) * 1000 << " ms\n";
    timefile.close();
  }

  delete[] rbuf;

  MPI_Finalize();
}