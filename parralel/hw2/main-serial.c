#include "helper.h"
#include "time.h"
#include <pthread.h>
#define M_PI 3.14159265358979323846
float result = 0;
float map_func2(int current_rank, int world_size);
bool filter_even(float x);
float * MPI_Map_Func_Serial(float *arr, int size,int current_process,int worldsize, float (*func)(int,int)); 
void * helper(void * arg);

struct monte_carlo_struct{
    int trial_size;
    int current_process;
    int number_of_processes;
};

int main(int argc, char **argv)
{
 double start,end;
 int trial_size;
 struct timeval st, et;
      trial_size = atoi(argv[2]);
      int world_size = atoi(argv[1]);
      float arr[(trial_size/world_size)*2];
      struct monte_carlo_struct *all= (struct monte_carlo_struct *)calloc(world_size , sizeof(struct monte_carlo_struct));
    
    pthread_t tids[world_size];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
    gettimeofday(&st,NULL);
    

    for (int i = 0; i < world_size; i++)
    {
        all[i].current_process = i;
        all[i].number_of_processes = world_size;
        all[i].trial_size= trial_size;
		pthread_create(&tids[i], &attr, helper,(void *) (&all[i]));
      
	}
    // Wait until thread is done its work
    gettimeofday(&et,NULL);
    
	for (int i = 0; i < world_size; i++) {
		pthread_join(tids[i], NULL);
    }

    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
      float found = (result/(float)trial_size)*4;
      fprintf(stderr,"result: %f in # of process: %d, with trial size %d, while error: %f elapsed time: %d İN SERİAL EXECUTİON\n",found,world_size,trial_size, fabs(M_PI-found)/M_PI, elapsed);
      //printf("All processes took %d seconds\n",elapsed);

      FILE *fr;
      fr = fopen("main-serial.txt", "a");
      if (fr == NULL){
        printf("Error Reading File\n");
        exit (0);
      }
     int ch=0;
     while ((ch = fgetc(fr)) != EOF){}
      fprintf(fr,"result: %f in # of process: %d, with trial size %d, while error: %f elapsed time: %d İN SERİAL EXECUTİON\n",found,world_size,trial_size, fabs(M_PI-found)/M_PI, elapsed);
      fclose(fr);
      /*
      float arr[(trial_size/world_size)*2];
      float result2;
      MPI_Map_Func(arr,(trial_size/world_size)*2,map_func);
      result2 = *MPI_Filter_Func(arr,(trial_size/world_size)*2,isInUnitCircle);
   */
    return 0;
}

float map_func2(int current_rank, int world_size){
    //srand ( time(NULL) );
    float addition = ((float)current_rank/(float)world_size);
    return ((float)rand()/(RAND_MAX)/world_size)+addition;
}
float * MPI_Map_Func_Serial(float *arr, int size,int current_process,int worldsize, float (*func)(int,int)){
    //srand ( time(NULL) );
    for (size_t i = 0; i < size; i++)
    {
        if(i%2 == 0){
            arr[i] = (float)rand()/RAND_MAX;
        }else
        {
            arr[i] = func(current_process,worldsize);
        }
        
        
    }
    return arr;
}
bool filter_even(float x){
    return ((int)x)%2 == 0;
}
void * helper(void * arg){
    struct monte_carlo_struct *arg_struct = ((struct monte_carlo_struct *) arg);
    
    int trial_size =arg_struct->trial_size;
    int current_process = arg_struct->current_process;
    int number_of_processes = arg_struct->number_of_processes;
    float arr[(trial_size/number_of_processes)*2];
    MPI_Map_Func_Serial(arr,(trial_size/number_of_processes)*2,current_process,number_of_processes,map_func2);
      
    result += *MPI_Filter_Func(arr,(trial_size/number_of_processes)*2,isInUnitCircle);
}