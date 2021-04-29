#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>

float *MPI_Map_Func(float *arr, int size, float (*func)(float));
float MPI_Fold_Func(float *arr, int size, float initial_value, float (*func)(float,float));
float *MPI_Filter_Func(float *arr, int size, bool (*pred)(float));
bool isInUnitCircle(float x);
float fold_add(float x, float y);

float fold_add(float x, float y){
    return x+y;
}
bool isInUnitCircle(float x) 
{ 
    // Compare radius of circle with distance  
    // of its center from given point 
    return x <= 0.5;
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

float * MPI_Map_Func(float *arr, int size, float (*func)(float)){
    //srand ( time(NULL) );
    for (size_t i = 0; i < size; i++)
    {
        if(i%2 == 0){
            arr[i] = (float)rand()/RAND_MAX;
        }else
        {
            arr[i] = func(size);
        }
        
        
    }
    return arr;
}
float MPI_Fold_Func(float *arr, int size, float initial_value, float (*func)(float,float)){
    float count = 0.0;
    //float * temp = MPI_Filter_Func(arr,size,isInUnitCircle);
    printf("\033[1;31m");
    for (size_t i = 0; i < size; i= i+2)
    {
        //printf("%f\t",(arr[i]));
        if(arr[i] = initial_value)
        {
            
            count = func(arr[i],count);
        }
        printf("\033[1;31m");
        printf("%f\t",(arr[i]));
         
    }
    printf("\033[0m;");
    return count;
}
float *MPI_Filter_Func(float *arr, int size, bool (*pred)(float)){
        //printf("\ncccccccccccccccccccccccccccccccccccccccc   %f % f\n",arr[0], arr[1]);    
    
    float * resultarr;
    //float result;

    resultarr = (float*)malloc(size * sizeof(float));
    int var = 0;
    int yok = 0;
    for (size_t i = 0; i < size; i = i+2 )
    {
         
        if(pred( (float)sqrt( 
            ( ( 0.5- arr[i] ) * ( 0.5- arr[i] ) ) 
            + 
            ( ( 0.5 - arr[i+1] ) * ( 0.5 - arr[i+1] ) ) 
            ) 
            ) ) {
            resultarr[i] = 1.0;
            resultarr[i+1] = 0.0;
            var++;
            //fprintf(stderr,"BULDU  %f\n",( (arr[i] - 0.5) * (arr[i] - 0.5) + (arr[i+1] - 0.5) * (arr[i+1] - 0.5) ));
        }
        else
        {
            yok++;
            resultarr[i] = 0.0;
            resultarr[i+1] = 0.0;
            //fprintf(stderr,"Yok\n");
        }
        
    }
    float *result = calloc(1, sizeof(float) );
    //float adress =MPI_Fold_Func(resultarr,size,1.0,fold_add);

    result[0] =(float)var;
    //fprintf(stderr,"Ne var: %d Ne Yok: %d  in size: %d\n",var,yok,size);
    
    //fprintf(stderr,"2Ne var: %d Ne Yok: %d  in size: %d\n",var,yok,size);
    float resultarr2[]={var,yok};
    return result;
}



