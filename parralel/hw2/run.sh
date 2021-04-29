gcc main-serial.c -o b -lpthread -lm
./b 1 "10"
./b 1 "100"
./b 1 "1000"
./b 1 "10000"
./b 1 "100000"
./b 1 "1000000"


./b 2 10
./b 2 100
./b 2 1000
./b 2 10000
./b 2 100000
./b 2 1000000

./b 4 10
./b 4 100
./b 4 1000
./b 4 10000
./b 4 100000
./b 4 1000000

./b 8 10
./b 8 100
./b 8 1000
./b 8 10000
./b 8 100000
./b 8 1000000


./b 16 10
./b 16 100
./b 16 1000
./b 16 10000
./b 16 100000
./b 16 1000000

./b 32 10
./b 32 100
./b 32 1000
./b 32 10000
./b 32 100000
./b 32 1000000


mpicc -o main main.c -lm


mpirun -np 1 main 10
mpirun -np 1 main 100
mpirun -np 1 main 1000
mpirun -np 1 main 10000
mpirun -np 1 main 100000
mpirun -np 1 main 1000000


mpirun -np 2 main 10
mpirun -np 2 main 100
mpirun -np 2 main 1000
mpirun -np 2 main 10000
mpirun -np 2 main 100000
mpirun -np 2 main 1000000

mpirun -np 4 main 10
mpirun -np 4 main 100
mpirun -np 4 main 1000
mpirun -np 4 main 10000
mpirun -np 4 main 100000
mpirun -np 4 main 1000000

mpirun -np 8 main 10
mpirun -np 8 main 100
mpirun -np 8 main 1000
mpirun -np 8 main 10000
mpirun -np 8 main 100000
mpirun -np 8 main 1000000

mpirun -np 16 main 10
mpirun -np 16 main 100
mpirun -np 16 main 1000
mpirun -np 16 main 10000
mpirun -np 16 main 100000
mpirun -np 16 main 1000000

mpirun -np 32 main 10
mpirun -np 32 main 100
mpirun -np 32 main 1000
mpirun -np 32 main 10000
mpirun -np 32 main 100000
mpirun -np 32 main 1000000