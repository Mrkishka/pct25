#!/bin/bash

mpicxx ring.cpp -o allgather

for np in {1..6}; do

  echo ""
  echo " ----------------------------------------"
  echo "|Executing 'AllGather' on $np threads|"
  echo " ----------------------------------------"
  echo ""

  mpirun -np $np ./allgather
done

rm allgather

echo ""
echo "Ring (AllGather) execution completed. Check ring_output.txt and ring_time.txt files for results."