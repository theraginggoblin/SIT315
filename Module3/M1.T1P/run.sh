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

rm -f matrix_multiply.cl && vi matrix_multiply.cl
rm -f matrix_multiply.cpp && vi matrix_multiply.cpp
mpic++ matrix_multiply.cpp -l OpenCL -o matrix_multiply
