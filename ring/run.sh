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
echo "Gotovo, glyan'te files"