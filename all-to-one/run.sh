#!/bin/bash

mpicxx all-to-one.cpp -o ato

for np in {1..6}; do
  echo ""
  echo "--------------------------------------"
  echo "Executing 'all to one' on $np threads"
  echo "--------------------------------------"
  echo ""

  mpirun -np $np ./ato
done

rm ato

echo ""
echo "All-to-One execution completed. Check all-to-one_output.txt and all-to-one_time.txt files for results."