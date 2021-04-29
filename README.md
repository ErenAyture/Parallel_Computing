# Parallel_Computing
These Projects are witten in C language.
**For First MPI**
I realized that due to message passing work load increases among process. They have to wait
until they receive the data. In first serial executions ı have found that making more thread
creates an overhead due to small size and programm cannot utilize from the implicit
parallelism. In each work overhead increased due to te overheads of creation of threads and
message passing time. I could finish min serial, min-mpi-v1, min-mpi-v2 and
greyscale-serial. Plots are given below. In addition , while broadcasting processes receive
data at the same time without a loop and MPI doe find min with its own library without
overhead.

**For Second MPI**
In following sections, i will be discussing about monte carlo simulation which are
parallel and serial execution. First I will discuss what kind of parallelism, does my
strategy apply for approximating pi in terms of data and task parallelism; then, what
is my communication scheme, along with MPI communication functions did i use
and why; How MPI address space is used between processes; What would be in a
distributed environment where i had many different computers connected together?
Would function pointers, to part of the code segment of virtual memory space, still
work if you had a truly distributed environment. and my timing results and my
graphs according to number of processes.

**For OMP**
I have used triplet data structure in the form of COO format representation.
Then shown in the below figure. I used a 2d array for the result vector due to
preventing race condition. However, each thread is parallelised according to the size
of the value pointer array. After dividing the work, the result vector takes an index
from the row pointer array. Therefore, there could be many same indexes at the
same time, It can be retrieved from different threads. To prevent that first, There are
number of precess, number of resulting vector that each thread works without race
condition than these results vectors are reduced in real result vector.

result_vector​ [ ​ omp_get_thread_num​ ()][​ row_ptr​ [i]-​ 1 ​ ] += (​ x ​ [ ​ col_ptr​ [i]-​ 1 ​ ] * ​ val_ptr​ [i]);

First ı have calculated Tp,Ts, S,E as
T_s = overalTime − f
T_p = f
T_p = overallTime − f + f/#p 
S = 1/Tp
E = S/#p

**For Cuda**
First I created x and y arrays that each represents a vector. Their size equals to n. If
there is no input file numbers are generated between 0 and 100 randomly, else read
from .txt file.
As math
c osθ = (U * V) / (|U||V|)
U is X array
V is Y array

U*V = X[0]*Y[0]+X[1]*Y[1]+X[2]*Y[2]+...X[N-1]*Y[N-1]
|U| = √ x[0] ^2 + x [1] ^2 + x [2] ^2 + x [3] ^2 + x [4] ^2 + . .. + x [N − 1 ] ^2
|V| = √ y[0] ^2 + y [1] ^2 + y [2] ^2 + y [3] ^2 + y [4] ^2 + . .. + y [N − 1 ] ^2


To do this operation I used mult kernel that i implemented. This kernel basically
computes Z[i] = x[i] * Y[i]. To gain the best performance of it, I used grid stride loop.
This kernel computes x [i] 2 , y [i] 2 , x [i] * y [i] . Their each result is stored in gpu global
memory with three different arrays to calculate U*V, |U|, |V| after. to calculate U*V,
|U|, |V| . Those three arrays are reduced by sum operation. Reduction follows the
strategy of sequential addressing, and stride loop is changed with reverse loop.
Results are stored and sent to the host. Now, we have, U * V ; | U |^ 2 ; | V |^ 2 separately.
Left part is math.

