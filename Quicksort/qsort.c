/* 
   To check correctness adjust N on line 8 and
   uncomment the print statements at 89 and 129
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 10000         // default set to 10000, change to 10 for checking
double startTime;
static inline

void swap(int * v, int i, int j)
{
    int t = v[i];
    v[i] = v[j];
    v[j] = t;
}

void quicksort(int * v, int s, int n)
{
    int x, p, i;
    if (n <= 1)
        return;
    x = v[s + n/2];
    swap(v, s, s + n/2);
    p = s;
    for (i = s+1; i < s+n; i++)
        if (v[i] < x) {
            p++;
            swap(v, i, p);
        }
    swap(v, s, p);
    quicksort(v, s, p-s);
    quicksort(v, p+1, s+n-p-1);
}

int * merge(int * v1, int n1, int * v2, int n2)
{
    int * result = (int *)malloc((n1 + n2) * sizeof(int));
    int i = 0, j = 0, k;

    for (k = 0; k < n1 + n2; k++) {
        if (i >= n1) {
            result[k] = v2[j];
            j++;
        }
        else if (j >= n2) {
            result[k] = v1[i];
            i++;
        }
        else if (v1[i] < v2[j]) {
            result[k] = v1[i];
            i++;
        }
        else {
            result[k] = v2[j];
            j++;
        }
    }
    return result;
}

int main(int argc, char ** argv)
{
    int n;
    int * data = NULL;
    int c, s;
    int * chunk;
    int o;
    int * other;
    int step, p, id;

    MPI_Status status;
    double elapsed_time;
    FILE * file = NULL;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0) {
        n=N;
        c = (n%p!=0) ? n/p+1 : n/p;
        data = (int *)malloc(p*c * sizeof(int));
        for (i = 0; i < n; i++)
            data[i] = rand()%1000000;
  //              printf("\n");
  //              for (i = 0; i < n; i++)
  //                  printf("%d ", data[i]);
  //              printf("\n");
        for (i = n; i < p*c; i++)
            data[i] = 0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = - MPI_Wtime();
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    c = (n%p!=0) ? n/p+1 : n/p;
    chunk = (int *)malloc(c * sizeof(int));
    MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
    free(data);
    data = NULL;
    s = (n >= c * (id+1)) ? c : n - c * id;
    quicksort(chunk, 0, s);

    for (step = 1; step < p; step = 2*step) {
        if (id % (2*step) != 0) {
            MPI_Send(chunk, s, MPI_INT, id-step, 0, MPI_COMM_WORLD);
            break;
        }
        if (id+step < p) {
            o = (n >= c * (id+2*step)) ? c * step : n - c * (id+step);
            other = (int *)malloc(o * sizeof(int));
            MPI_Recv(other, o, MPI_INT, id+step, 0, MPI_COMM_WORLD, &status);
            data = merge(chunk, s, other, o);
            free(chunk);
            free(other);
            chunk = data;
            s = s + o;
        }
    }

    elapsed_time += MPI_Wtime();

    if (id == 0) {
        printf("\nRunning quicksort using %d core(s)!\n", p);
        printf("\n");
//                for (i = 0; i < s; i++)
//                    printf("%d ", chunk[i]);
//                printf("\n\n");
        printf("Completed in %f seconds!\n\n", elapsed_time);
    }

    MPI_Finalize();
    return 0;
}
