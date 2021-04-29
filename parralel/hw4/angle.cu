#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h> 
#define SIZE 1<<10
#define SHMEM_SIZE 1024 * 8
#define PI 3.14159265

__global__
void mult(int n, double *x, double *y, double *z)
{
  //extern __shared__ int sdata[]; 
  //int i = blockIdx.x*blockDim.x + threadIdx.x;
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for (int i = index; i < n; i += stride){
    z[i] = x[i] * y[i];
  }
}

__global__ void sum_reduction(double *v, double *v_r) {
	// Allocate shared memory
	extern __shared__ double partial_sum[];

	// Calculate thread ID
	//int tid = blockIdx.x * blockDim.x + threadIdx.x;

	// Load elements AND do first add of reduction
	// Vector now 2x as long as number of threads, so scale i
	int i = blockIdx.x * (blockDim.x * 2) + threadIdx.x;

	// Store first partial result instead of just the elements
	partial_sum[threadIdx.x] = v[i] + v[i + blockDim.x];
	__syncthreads();

	// Start at 1/2 block stride and divide by two each iteration
	for (int s = blockDim.x / 2; s > 0; s >>= 1) {
		// Each thread does work unless it is further than the stride
		if (threadIdx.x < s) {
			partial_sum[threadIdx.x] += partial_sum[threadIdx.x + s];
		}
		__syncthreads();
	}

	// Let the thread 0 for this block write it's result to main memory
	// Result is inexed by this block
	if (threadIdx.x == 0) {
		v_r[blockIdx.x] = partial_sum[0];
	}
}


double CPU_angle(double * x, double * y, int n);
double CPU_angle(double * x, double * y, int n){
  double sum1 = 0;
  double sum2 = 0;
  double sum3 = 0;

  for(int i = 0; i < n; i++){
      sum1 += ( x[i] * y[i] );
      sum2 += ( x[i] * x[i] );
      sum3 += ( y[i] * y[i] );
  }

  double val = 180.0 / PI;
	double det = sqrt(sum2)*sqrt(sum3) ;
  double result = atan2( det, sum1 ) *val;
  return result;
}

int main(int argc, char *argv[]) {
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  int n = 1;
  int b_size;

  if(argv[1] != NULL){
    n = atoi(argv[1]);
    
  }
  if(argv[2] != NULL){
    #undef SIZE 
    #define SIZE  atoi(argv[2])
    #undef SHMEM_SIZE
    #define SHMEM_SIZE SIZE*4

  }
  FILE *myFile;
  //printf("%s",argv[3]);
  if(argv[3] !=NULL){
    
    
    myFile = fopen(argv[3], "r");
    fscanf(myFile,"%d",&n);
    n = n/2;
    //nvccprintf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
  }
  b_size = (n+SIZE-1)/SIZE;
  
  printf("Info\n");
  printf("——————\n");
  printf("Number of elements: %d\n",n);
  printf("Number of threads per block: %d\n",SIZE);
  printf("Number of blocks will be created: %d\n\n",b_size);
  
  size_t bytes = n * sizeof(double);
  
  //printf("%d",b_size);
  
  cudaEventRecord(start);
  double *x, *y,*d_x, *d_y;
  
  x = (double*)malloc(bytes);
  y = (double*)malloc(bytes);
  
  double* multiplications, * square1, * square2,* d_multiplications, * d_square1, * d_square2;
  
  multiplications = (double*)malloc(bytes);
  square1 = (double*)malloc(bytes);
  square2 = (double*)malloc(bytes);

  double *uv,*d_uv;
  double *IuI, *IvI, *d_IuI, *d_IvI; 
  uv = (double*)malloc(bytes);
  IuI = (double*)malloc(bytes);
  IvI = (double*)malloc(bytes);
  
  cudaMalloc(&d_x, bytes); 
  cudaMalloc(&d_y, bytes);
  cudaMalloc(&d_multiplications, bytes); 
  cudaMalloc(&d_square1, bytes);
  cudaMalloc(&d_square2, bytes); 
  cudaMalloc(&d_IuI,bytes);
  cudaMalloc(&d_IvI,bytes);
  cudaMalloc(&d_uv,bytes);
  for (int i = 0; i < n; i++) {
    multiplications[i] = 0;
    square1[i]=0;
    square2[i]=0;
    IuI[i]= 0;
    IvI[i]= 0;
    uv[i] = 0;
  }
  if(argv[3] != NULL){
    for (size_t i = 0; i < n; i++){
      fscanf(myFile,"%lf",&x[i]);
    }
    for (size_t i = 0; i < n; i++){
      fscanf(myFile,"%lf",&y[i]);
    }
    fclose(myFile); 
     
  }else{
    
    for (int i = 0; i < n; i++) {
      //srand(time(0));
      x[i] = rand() % 100;
      //printf("%d ", x[i]);
      y[i] = rand() % 100;
      
    }  
  }

  
  
  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  float milliseconds = 0;
  cudaEventElapsedTime(&milliseconds, start, stop);

  

  printf("Time\n");
  printf("——————\n");
  printf("Time for the array generation :%f ms\n",milliseconds);

  cudaEventRecord(start);
  double cpu_result = CPU_angle(x,y,n);
  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&milliseconds, start, stop);
  printf("Time for the CPU function : %f ms\n",milliseconds);

  cudaEventRecord(start);

  cudaMemcpy(d_x, x, bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_y, y, bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_multiplications, multiplications, bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_square1, square1, bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_square2, square2, bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_IuI,IuI,1*sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(d_IvI,IvI,1*sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(d_uv,uv,1*sizeof(double), cudaMemcpyHostToDevice);

  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  milliseconds = 0;
  cudaEventElapsedTime(&milliseconds, start, stop);
  printf("Time for the Host toDevice transfer : %f ms\n",milliseconds);

  cudaEventRecord(start);

  mult<<<b_size, SIZE>>>(n, d_x, d_y,d_multiplications);
  mult<<<b_size, SIZE>>>(n, d_x, d_x, d_square1);
  mult<<<b_size, SIZE>>>(n, d_y, d_y, d_square2);


	// Original vector and result vector
	


	// TB Size
	int TB_SIZE = SIZE;

	// Grid Size (cut in half) (No padding)
	int GRID_SIZE = (n / (TB_SIZE) / 2) ;

	// Call kernel
	sum_reduction << <GRID_SIZE, TB_SIZE, TB_SIZE*8 >> > (d_multiplications, d_uv);

  sum_reduction << <1, TB_SIZE, TB_SIZE*8 >> > (d_uv, d_uv);
    

  sum_reduction << <GRID_SIZE, TB_SIZE, TB_SIZE*8 >> > (d_square1, d_IuI);

  sum_reduction << <1, TB_SIZE, TB_SIZE*8 >> > (d_IuI, d_IuI);
    
    
  sum_reduction << <GRID_SIZE, TB_SIZE, TB_SIZE*8 >> > (d_square2, d_IvI);

	sum_reduction << <1, TB_SIZE, TB_SIZE*8 >> > (d_IvI, d_IvI);

  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  float milliseconds2 = 0;
  cudaEventElapsedTime(&milliseconds2, start, stop);
  printf("Time for the kernel execution :%f ms\n",milliseconds2);
  
  // Copy to host;
  cudaEventRecord(start);
	cudaMemcpy(uv,d_uv, bytes, cudaMemcpyDeviceToHost);
  cudaMemcpy(IuI,d_IuI, bytes, cudaMemcpyDeviceToHost);
  cudaMemcpy(IvI,d_IvI, bytes, cudaMemcpyDeviceToHost);
  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  float milliseconds3 = 0;
  cudaEventElapsedTime(&milliseconds3, start, stop);
  printf("Time for the Deviceto Host transfer :%f ms\n",milliseconds3);
  printf("Total execution time for GPU: %f ms\n\n",milliseconds+milliseconds2+milliseconds3);
  double val = 180.0 / PI;
	double det = sqrt(IuI[0])*sqrt(IvI[0]) ;
  double gpu_result = atan2(det, uv[0] ) *val;

  
  
  printf("Results\n");
  printf("——————\n");
  printf("CPU Result: %lf\n", cpu_result);
  printf("GPU Result: %lf\n\n", gpu_result);
  
  cudaFree(d_IuI);
  cudaFree(d_IvI);
  cudaFree(d_multiplications);
  cudaFree(d_square1);
  cudaFree(d_square2);
  cudaFree(d_uv);
  cudaFree(d_x);
  cudaFree(d_y);

  free(x);
  free(y);
  free(IuI);
  free(IvI);
  free(uv);
  free(multiplications);
  free(square1);
  free(square2);


	return 0;
}
