#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>
int min = INT_MAX;

pthread_mutex_t lock_a, lock_b, lock_c, lock_d;


// Thread function to generate sum of 0 to N


struct sum_partitioned_struct {
	int *arr;
	int p;
    int r;
    int min;
    int proces;
};
void * partitioned_min(void * arg){
    struct sum_partitioned_struct *arg_struct = ((struct sum_partitioned_struct *) arg);
    int proc = arg_struct->proces;
    int p = arg_struct->p;
    int r = arg_struct->r;
    
    fprintf(stderr,"p inside: %d r inside: %d in process %d\n",p,r,proc);
    if(arg_struct->p>=arg_struct->r ){
        return;
    }
    
    int minimum = arg_struct->arr[p]; 
    for (size_t i = p; i <= r; i++)
    {
        if (arg_struct->arr[i] < minimum)
        {
            //fprintf(stderr,"%d %d",minimum,min);
            minimum = arg_struct->arr[i];
            //fprintf(stderr,"Min changed inside inside: %d  in process %d\n",min,proc);
        }
               
    }
    
    if(min > minimum){
            min = minimum;
            fprintf(stderr,"Min changed inside inside: %d  in process %d\n",min,proc);
        } 
    
}

int main(int argc, char **argv)
{

	if (argc < 2) {
		printf("Usage: %s <num 1> <num 2> ... <num-n>\n", argv[0]);
        printf("Min changed inside inside: %d  in process %d\n",min,min);  
	//	exit(-1);
	}
	
    struct timeval st, et;
  
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
    int numberArray[--lines];
    fclose(myFile);
    myFile = fopen("input.txt", "r");
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    for (size_t i = 0; i < lines; i++){
        fscanf(myFile, "%d,", &numberArray[i] );
    }
    /*
    for (size_t i = 0; i < lines; i++){
        fprintf(stderr,"a[%d] = %ld\n",i,numberArray[i]);
    }
    */
    fprintf(stderr,"aaaaaaaaa\n");
    
    
    gettimeofday(&st,NULL);
    //partitioned_min(numberArray,0,99,&min);
    
    //
    
	// Launch thread
	int number_of_threads = 8;

    struct sum_partitioned_struct *all= (struct sum_partitioned_struct *)calloc(number_of_threads , sizeof(struct sum_partitioned_struct));
    
    
    
    //struct sum_partitioned_struct all[number_of_threads];
    pthread_t tids[number_of_threads];
    //struct sum_partioned_struct args[num_args];
    /*
    for (size_t i = 0; i < lines; i++){
        fprintf(stderr,"a[%d] = %d\n",i,all[i]].arr[i]);
    }
    */
    
	pthread_attr_t attr;
	pthread_attr_init(&attr);
    int j = 0;

    for (int i = 0; i < number_of_threads && j < lines; i++)
    {
        all[i].arr = numberArray;
        all[i].min = min;
        all[i].proces = i;
        all[i].arr = numberArray;
        all[i].p = j;
        all[i].r = j + lines/number_of_threads;
        if(all[i].r>=100){
            all[i].r=99;
        }
        fprintf(stderr,"p:%d r:%d i:%ld\n",all[i].p,all[i].r,i);

		
		pthread_create(&tids[i], &attr, partitioned_min,(void *) (&all[i]));
      
        //min = all[i].min;

        j = j + lines/(number_of_threads);
	}
    fprintf(stderr,"Minimum is: %d\n",min);
	// Wait until thread is done its work
    gettimeofday(&et,NULL);
    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    fprintf(stderr,"Elapsed time is%d\n",elapsed);
	for (int i = 0; i < number_of_threads; i++) {
		pthread_join(tids[i], NULL);
		
	}
    
   return 0;
}
