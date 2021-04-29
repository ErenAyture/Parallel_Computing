#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc, char *argv[]){ 

    double startTs; 
    double endTs; 

    startTs =omp_get_wtime();


    int number_of_threads;
    number_of_threads = atoi(argv[1]);
    int number_of_iterations = atoi(argv[2]); 
    int bool_switch = atoi(argv[3]);
    char * testURL = argv[4];
    
        omp_set_num_threads(number_of_threads);

    
    FILE *myFile;
    myFile = fopen(testURL, "r");
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    int lines=0;
    int ch=0;
    while ((ch = fgetc(myFile)) != EOF)
        {
            if (ch == '\n')
            lines++;
        }
    fclose(myFile);

    myFile = fopen(testURL, "r");
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    int rowSize;
    int colSize;
    int valSize;


    fscanf(myFile,"%d %d %d",&rowSize,&colSize,&valSize);
    
    int *row_ptr = (int *) malloc(valSize * sizeof(int));
    int *col_ptr  = (int *) malloc(valSize * sizeof(int));
    long double *val_ptr  = (long double *) malloc(valSize * sizeof(long double));
    
    for (size_t j = 0; j < valSize; j++)
        {
            row_ptr[j] = 0;
            col_ptr[j] = 0;
            val_ptr[j] = 0;
        }

    for (size_t i = 0; i < lines-1; i++){
        fscanf(myFile,"%d %d %Le",&row_ptr[i],&col_ptr[i],&val_ptr[i]);      
    }

    fclose(myFile);

    int x[colSize];
    for (size_t i = 0; i < colSize; i++)
    {
        x[i] = 1;
    }
    long double **result_vector = malloc(sizeof(long double*) *number_of_threads);
    for (size_t i = 0; i < number_of_threads; i++)
    {
        result_vector[i] = malloc(sizeof(long double)*colSize);
    }
    
    for (size_t i = 0; i < number_of_threads; i++)
    {
        for (size_t j = 0; j < colSize; j++)
        {
            result_vector[i][j] = 0;
        }
        
    }

    
    double start; 
    double end; 
    
     
    long double result[colSize];
    for (size_t j = 0; j < colSize; j++)
        {
            result[j] = 0;
        }
    

    endTs =omp_get_wtime();
    //printf("Work took %f seconds(Ts) in number \n", endTs - startTs, number_of_threads);

    omp_lock_t simple_lock;
    /*
        Parallel Region
    */
    #pragma omp parallel shared(x,row_ptr,result_vector,col_ptr,val_ptr,valSize)  default(shared)
    {
        start = omp_get_wtime(); 

        //long double local_result[colSize];
        #pragma omp parallel for schedule(static) //reduction(+:result[:colSize])
        for (int i = 0; i < valSize; i++)
        {
            //#pragma omp critical( result_vector)
            //{       
                //omp_test_lock( &simple_lock );
                //#pragma omp atomic
                result_vector[omp_get_thread_num()][row_ptr[i]-1] += (x[col_ptr[i]-1] * val_ptr[i]);
                //omp_unset_lock( &simple_lock);
                
            //}
            
            //result[i] += result_vector[omp_get_thread_num()][row_ptr[i]-1];
        }

        //#pragma omp parallel for schedule(static) reduction(+:result[:colSize])
        
        end = omp_get_wtime();

        /*Reduction*/
        for (size_t i = 0; i < colSize; i++)
        {
            
            for (size_t j = 0; j < number_of_threads; j++)
            {   
                //#pragma omp critical
                result[i] += result_vector[j][i];
            }
            
        }
    }
    
    /*
    for (int i = 0; i < valSize; i++)
        {
            //#pragma omp atomic update
            result_vector[row_ptr[i]-1] += (x[col_ptr[i]-1] * val_ptr[i]);
        }
    */
   /*Reduction*/
   for (size_t i = 0; i < colSize; i++)
        {
            
            for (size_t j = 0; j < number_of_threads; j++)
            {   
                //#pragma omp critical
                result[i] += result_vector[j][i];
            }
            
        }
    if(bool_switch==1){
        fprintf(stderr,"INPUT MATRIX TAKEN FROM: %s\n\n",testURL);
        for (size_t i = 0; i < valSize; i++){
            fprintf(stderr,"%d %d %Le \n",row_ptr[i],col_ptr[i],val_ptr[i]);      
        }
        
        fprintf(stderr,"Vector to be Multiplied\n\n");
        for (size_t i = 0; i < colSize; i++){
            fprintf(stderr,"%d\n",x[i]);      
        }
        fprintf(stderr,"Resulting Vector\n\n");    
        for (size_t i = 0; i < colSize; i++)
        {
            fprintf(stderr,"%Le\n",result[i]);
        }

    }
    double Tp;
    Tp =  (endTs - startTs)+ ((end - start)/number_of_threads);
    double s;
    s = 1/Tp;
    double e = s/number_of_threads;
    printf("Work took Tp: %f seconds with S: %f , E: %f in %d number  of threads\n",Tp, s , e , number_of_threads);

    free(col_ptr);
    free(row_ptr);
    free(val_ptr);
    for(int i = 0; i < number_of_threads; i++)
        free(result_vector[i]);
    free(result_vector);
    return 0;
}

