/* 
   To show correctness during evals set 
   sizes = 4 on line 11  and uncomment printf on line 78
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define NRA 200
#define NCA 200
#define NCB 200
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main (int argc, char *argv[])
{
    int numtasks, taskid, numworkers, source, dest, mtype, rows;
    int averow, extra, offset, i, j, k, rc;

    double  a[NRA][NCA],           /* matrix A to be multiplied */
            b[NCA][NCB],           /* matrix B to be multiplied */
            c[NRA][NCB];           /* result matrix C */
    
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    numworkers = numtasks-1;

    if (taskid == MASTER)
    {
        for (i=0; i<NRA; i++)                   // MATRIX HAVE BEEN SET HERE
            for (j=0; j<NCA; j++)
                a[i][j]= i+j;

        for (i=0; i<NCA; i++)
            for (j=0; j<NCB; j++)
                b[i][j]= i*j;

        averow = NRA/numworkers;
        extra = NRA%numworkers;
        offset = 0;
        mtype = FROM_MASTER;
        
        for (dest=1; dest<=numworkers; dest++)
        {
            rows = (dest <= extra) ? averow+1 : averow;    
            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,
                    MPI_COMM_WORLD);
            MPI_Send(&b, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset = offset + rows;
        }

        mtype = FROM_WORKER;
        clock_t start, end;
        start = clock();
        for (i=1; i<=numworkers; i++)
        {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, mtype, 
                    MPI_COMM_WORLD, &status);
        }

        end=clock();

        printf ("\nDoing matrix multiplication with %d core(s)!\n", numtasks/2);

        /*        printf("\nResult Matrix:\n");
                  for (i=0; i<NRA; i++)
                  {
                  printf("\n"); 
                  for (j=0; j<NCB; j++) 
                  printf("%6.2f   ", c[i][j]);
                  }
                  printf("\n");

         */
        printf ("\nCompleted in %lf seconds!\n\n", (double)(end-start)/CLOCKS_PER_SEC);
    }


    /**************************** worker task ************************************/
    if (taskid > MASTER)
    {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        for (k=0; k<NCB; k++)
            for (i=0; i<rows; i++)
            {
                c[i][k] = 0.0;
                for (j=0; j<NCA; j++)
                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
            }
        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
