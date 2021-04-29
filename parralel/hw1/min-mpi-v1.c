#include <stdio.h>
#include "mpi.h"
#include <limits.h>

typedef struct partitioned_struct {
	int length;
	int p;
    int r;
} vals;

void partitioned_min(int * arr,int p, int r ,int * minimum){
    
    *minimum = INT_MAX;
    fprintf(stderr,"p: %d r: %d \n",p,r);
    for (int i = p; i <= r; i++)
    {
        if(arr[i] < *minimum){
            *minimum = arr[i];
        }
    }
}
int main (int argc, char *argv[])
{ MPI_Status s;
 int size, rank, i, j;
 int k = 0;
 int ss = 0;
double start, end;
 MPI_Init (&argc, &argv);
 MPI_Comm_size (MPI_COMM_WORLD, &size);

const int nitems=3;
    int          blocklengths[3] = {1,1,1};
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype mpi_partitioned_type;
    MPI_Aint     offsets[3];

    offsets[0] = offsetof(vals, length);
    offsets[1] = offsetof(vals, p);
    offsets[1] = offsetof(vals, r);


    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_partitioned_type);
    MPI_Type_commit(&mpi_partitioned_type);

 
 int *numberArray;// = (int*) malloc(arrSize * sizeof(int));
 
 if (rank == 0) // Master process
 { 
    FILE *myFile;
    myFile = fopen("input.txt", "r");
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    int ch=0;
    int lines=0;
    lines++;
    while ((ch = fgetc(myFile)) != EOF)
        {
            if (ch == '\n')
            lines++;
        }
    //read file into array
    fprintf(stderr,"number of lines: %d\n",lines);
    //int numberArray[--lines];
    numberArray = (int*) malloc((--lines) * sizeof(int));
    fclose(myFile);
    myFile = fopen("input.txt", "r");
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    for (size_t i = 0; i < lines; i++){
        fscanf(myFile, "%d,", &numberArray[i] );
    }
    for (size_t i = 0; i < lines; i++){
        fprintf(stderr,"a[%d] = %ld\n",i,numberArray[i]);
    }
    fprintf(stderr,"aaaaaaaaa\n");

    printf ("Receiving data . . .\n");
    
    start = MPI_Wtime();
    for (i = 1; i < size; i++){
        ss = k + lines/(size-1);
        
        MPI_Send ((void *)&k, 1, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
        MPI_Send ((void *)&ss, 1, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
        MPI_Send ((void *)&lines, 1, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
        MPI_Send ((void *)numberArray, lines, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);


        k = ss+1; 
        }
    
    //int results[size-1];
    int results[size -1];
    //for(i = 1; i < size; i++ ){
        printf ("Receiving data . . .\n");
        for (i = 1; i < size; i++)
        { MPI_Recv ((void *)&results[i-1], 1, MPI_INT, i, 0xACE5, MPI_COMM_WORLD, &s);
        printf ("[%d] sent %d\n", i, results[i-1]);
        }
   
   int realMin = INT_MAX;
        
        for (int i = 0; i <= size-2; i++)
        {
            if(results[i] < realMin){
                realMin = results[i];
            }
        }
   fprintf(stderr,"Minimum is: %d \n",realMin);
   end = MPI_Wtime();
   printf("Our timers precision is %.20f seconds\n",MPI_Wtick());
 printf("This silly loop took %.5f seconds\n",end-start);
 }
 else{ int arrSize;
        MPI_Status s;
        int a ;
        int b ;
        MPI_Recv ((void *)&a,1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD,&s);
        MPI_Recv ((void *)&b,1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD,&s);
        MPI_Recv ((void *)&arrSize,1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD,&s);
        int numberArray[arrSize];
        if(b >= arrSize ){
            b = arrSize -1;
        }
        MPI_Recv ((void *)numberArray,arrSize, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD,&s);
        fprintf(stderr,"Received Size is %d from current rank: %d \n",arrSize   ,rank);
        
        fprintf(stderr,"p: %d  r:  %d   length: %d\n",a,b,arrSize);
        
       int minimum = INT_MAX;
        
        for (int i = a; i <= b; i++)
        {
            if(numberArray[i] < minimum){
                minimum = numberArray[i];
            }
        }
       MPI_Send ((void *)&minimum, 1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD);
    }
 MPI_Finalize();
 return 0;
}


