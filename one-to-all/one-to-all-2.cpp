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

  int msg_size = 6;
  MPI_Datatype datatype = MPI_CHAR;

  char *sbuf = new char[msg_size];
  char *rbuf = new char[msg_size];

  if (rank == 0)
  {
    std::strcpy(sbuf, "abcdef");
    for (int i = 1; i < num_procs; i++)
    {
      MPI_Send(sbuf, msg_size, datatype, i, 0, MPI_COMM_WORLD);
    }
    std::ofstream outfile("one-to-all-2_output.txt", std::ios::app);
    outfile << "Process " << rank << " sent message to all other processes."
            << std::endl;
    outfile.close();
  }
  else
  {
    MPI_Recv(rbuf, msg_size, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::ofstream outfile("one-to-all-2_output.txt", std::ios::app);
    outfile << "Process " << rank << " received message: " << rbuf
            << std::endl;
    outfile.close();
  }

  end_time = MPI_Wtime();

  if (rank == 0)
  {
    std::ofstream timefile("one-to-all-2_time.txt", std::ios::app);
    timefile << "Time for " << num_procs << " processes: "
             << (end_time - start_time) * 1000 << " ms\n";
    timefile.close();
  }

  delete[] sbuf;
  delete[] rbuf;

  MPI_Finalize();
}