# Compile and run instructions

1) normal.c - gcc normal.c && ./a.out
2) proc.c - gcc proc.c && ./a.out
3) thread.c - gcc -pthread proc.c && ./a.out



# Input and Output

All 3 codes take N and an array of size N as an input and then 
print the sorted array and total time taken to sort as an output.



# Explanations and observations

1) normal.c - simple mergesort without any concurrency
2) proc.c - concurrent mergesort which creates child processes to sort subarrays
2) thread.c - concurrent mergesort which creates threads to sort subarrays

For a sufficiently large N (>=1000), it is clear that normal.c takes the least time.
Concurrency doesn't give any improvement in time. Among proc.c and thread.c, thread.c takes 
lesser time as creating threads requires much less resources than creating processes.

Since our teams main goal was parallelising algorithms and reduce runtimes, we didn't
experiment with concurrency after this point as no improvement was seen.
