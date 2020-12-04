
# Short description of our project

Our team worked on parallelising various algorithms and improving their runtime. I picked algorithms such as Quick sort, Sieve of Eratosthenes etc. and parallelised their computation tasks. This involved analysing how an algorithm can be broken down and the individual segments can be worked out parallely. The algorithms were coded with and without parallelisation and we compared the results on a sample dataset. All the codes I have parallelised successfully give a better runtime when a higher number of cores are used. The written codes can be used as libraries corresponding to each algorithm.

# Contents of submission

This submission file contains all my codes in folders along with bash scripts to run the code. For each code sr.sh runs the code on a single core while dr.sh uses two cores.
All my study and observations have been recorded in the report file. The contents of the report file are as follows (in order) -
  - Teams's proposal
  - Difference between concurrency and parallelism
  - Experimenting with concurrency, OpenMP and MPI
  - Things to take care of while parallelising
  - Ideal case of parallelising
  - Expectations of runtime
  - Final conclusion
  - Brief summary of all algorithms we parallelised
  - Data record of average runtimes
  - Timeline of the project