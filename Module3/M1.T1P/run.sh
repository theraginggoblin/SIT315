rm main.cpp && vi main.cpp && mpic++ main.cpp -o main
mpic++ -fopenmp main.cpp -o main
mpirun -np 4 ./main
mpirun -host 192.168.142.129,192.168.142.128 -np 8 ./main

rm -f main.cpp && vi main.cpp && mpic++ main.cpp -o main
rm -f hello.cpp && vi hello.cpp && mpic++ hello.cpp -o hello
rm -f vectoradd.cpp && vi vectoradd.cpp && mpic++ vectoradd.cpp -o vectoradd
ulimit -s unlimited
rm -f main.cpp && vi main.cpp && mpic++ -fopenmp main.cpp -o main

ssh 192.168.142.129

rm -f matrix_multiply_mpi.cpp && vi matrix_multiply_mpi.cpp && mpic++ matrix_multiply_mpi.cpp -o matrix_multiply_mpi
mpirun -host 192.168.142.128,192.168.142.129 -np 2 ./matrix_multiply_mpi

rm -f matrix_multiply_openmp.cpp && vi matrix_multiply_openmp.cpp && mpic++ matrix_multiply_openmp.cpp -fopenmp -o matrix_multiply_openmp
mpirun -host 192.168.142.128,192.168.142.129 -np 2 ./matrix_multiply_openmp

rm -f matrix_multiply.cl && vi matrix_multiply.cl
rm -f matrix_multiply_opencl.cpp && vi matrix_multiply_opencl.cpp && mpic++ matrix_multiply_opencl.cpp -l OpenCL -o matrix_multiply_opencl
mpirun -host 192.168.142.128,192.168.142.129 -np 2 ./matrix_multiply_opencl