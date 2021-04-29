#include "helper.h"
#include "mpi.h"
#include "time.h"
#define M_PI 3.14159265358979323846
float map_square(float x);

bool filter_even(float x); 
//bool isInUnitCircle(float x);

/*
bool isInUnitCircle(float x) 
{ 
    // Compare radius of circle with distance  
    // of its center from given point 
    return x <= 0.5;
}
*/
float map_func(float x){
   int current_rank;
   int world_size;
   //srand((unsigned int)time(NULL));
    MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
      
    float addition = ((float)current_rank/(float)world_size);
    //printf("\n addition %f in current rank %d\n",addition,current_rank);
    //srand ( time(NULL) );
    return ((float)rand()/(RAND_MAX)/world_size)+addition;
}



bool filter_even(float x){
    return ((int)x)%2 == 0;
}
int main(int argc, char **argv)
{

 MPI_Status s;
 double start,end;
 int size, rank;
 int trial_size;
 MPI_Init (&argc, &argv);
 
 MPI_Comm_size (MPI_COMM_WORLD, &size);
MPI_Comm_rank (MPI_COMM_WORLD, &rank);



   if(rank == 0){
      start = MPI_Wtime();
      int trial_size = atoi(argv[1]);
      int world_size;
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);
      float arr[(trial_size/size)*2];
      
      for (int i = 1; i < world_size; i++){
         MPI_Send ((void *)&trial_size, 1, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);   
      }
      
      float result = 0;
      
      MPI_Map_Func(arr,(trial_size/world_size)*2,map_func);
      /*
      float * temp = MPI_Filter_Func(arr,(trial_size/world_size)*2,isInUnitCircle);
      float *temp2 = (float*)malloc((trial_size/world_size)*2 * sizeof(float)); ;
      for (size_t i = 0; i < (trial_size/world_size)*2; i++)
      {
         printf("\033[1;33m");
         temp2[i] = temp[i];
         printf("%f\t",(temp2[i]));
      }
      */
      result = *MPI_Filter_Func(arr,(trial_size/world_size)*2,isInUnitCircle);
      
      //fprintf(stderr,"result: %f in process: %d\n",result,rank);
      float result2 = 0;
      for (int i = 1; i < world_size; i++){
         
         MPI_Recv ((void *)&result2, 1, MPI_FLOAT, i, 0xACE5, MPI_COMM_WORLD,&s);
         result += result2;   
      }
      //fprintf(stderr,"RRRresult: %f in process: %f\n",result,(result/(float)trial_size)*4);

      float found = (result/(float)trial_size)*4;
      end = MPI_Wtime();
      fprintf(stderr,"result: %f in # of process: %d, with trial size %d, while error: %f in time: %.5f in PARALLEL EXECUTION\n",found,world_size,trial_size, 100*fabs(M_PI-found)/M_PI,end-start);
      

      FILE *fr;
      fr = fopen("main-parallel.txt", "a");
      if (fr == NULL){
        printf("Error Reading File\n");
        exit (0);
      }
     int ch=0;
     while ((ch = fgetc(fr)) != EOF){}
      fprintf(fr,"result: %f in # of process: %d, with trial size %d, while error: %f in time(s): %.5f in PARALLEL EXECUTION\n",found,world_size,trial_size, 100*fabs(M_PI-found)/M_PI,end-start);
      fclose(fr);
      
   }else{
      int trial_size;
      int world_size;
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);
      //fprintf(stderr,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
      MPI_Recv ((void *)&trial_size, 1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &s);
            //printf("\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa   %d\n",trial_size);
      //printf("\n trial size %d in process %d\n",trial_size, rank);
      //printf("\n world size %d in process %d\n",world_size, rank);
      float arr[(trial_size/world_size)*2];
      float result2;
      
      MPI_Map_Func(arr,(trial_size/world_size)*2,map_func);
      /*
      float * temp = MPI_Filter_Func(arr,(trial_size/world_size)*2,isInUnitCircle);
      float *temp2 = (float*)malloc((trial_size/world_size)*2 * sizeof(float)); ;
      for (size_t i = 0; i < (trial_size/world_size)*2; i++)
      {
         printf("\033[1;33m");
         temp2[i] = temp[i];
         printf("%f\t",(temp2[i]));
      }
      */
      result2 = *MPI_Filter_Func(arr,(trial_size/world_size)*2,isInUnitCircle);
      
      //fprintf(stderr,"result: %f in process: %d\n",result2,rank);
      MPI_Send ((void *)&result2,1,MPI_FLOAT, 0, 0xACE5, MPI_COMM_WORLD);
   }
   
    MPI_Finalize();
    return 0;
}