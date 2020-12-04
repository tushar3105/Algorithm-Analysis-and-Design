/* UNCOMMENT LINE 78 WHILE SHOWING TO SIR */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

long get_elem_at_index(long index, int rank, int p, long n){
    long first_elem = floor(rank*(n-2)/p) + 2;
    return first_elem + index;
}

int main(int argc, char* argv[]) {

    int p;
    int rank;
    int DEBUG = 1;
    unsigned long n;
    unsigned long first_elem;
    unsigned long last_elem;
    unsigned long size;
    unsigned long i;
    unsigned long k;
    unsigned long index_first_multiple;
    unsigned long next_k;
    unsigned int local_sum;
    unsigned int global_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double time = MPI_Wtime();

    if(!(argc == 2)){
        if(rank == 1) printf("Error: expected 1 or 2 arguments but got %d \n", argc-1);
        exit(1);
    }
    n = atol(argv[1]);

    if(!(n/p > sqrt(n))){
        if(rank == 1) printf("Error: there are too many processes! \n");
        MPI_Finalize();
        exit (1);
    }

    first_elem = floor(rank*(n-2)/p) + 2;
    last_elem = floor((rank+1)*(n-2)/p) - 1 + 2;
    size = last_elem - first_elem + 1;

    bool *array = new bool[size];
    for(i = 0; i < size; i++){
        array[i] = true;
    }

    k = 2;

    while(k*k <= n)
    {
        index_first_multiple;
        if(first_elem % k == 0) index_first_multiple = 0;
        else index_first_multiple = k - first_elem % k;

        for(i = index_first_multiple; i < size; i += k){
            array[i] = false;
        }
        if(rank == 0) array[k-2] = true;

        if(rank == 0){
            next_k = k+1;
            while(!array[next_k - 2]) next_k = next_k + 1;
            k = next_k;
        }

        MPI_Bcast (&k,  1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
//    for(i = 0; i < size; i++)
//        if(array[i]) printf("%lu \n", get_elem_at_index(i,rank,p,n));
    

    local_sum = 0;
    global_sum;
    for(i = 0; i < size; i++){
        if (array[i]) local_sum++;
    }
    MPI_Reduce (&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
    	printf("Running seive using %d core(s)!\n", p);
        time = MPI_Wtime() - time;
        printf("The algorithm took %f seconds to execute!\n", time);
    }

    delete(array);
    MPI_Finalize();
}
