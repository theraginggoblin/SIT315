rm main.cpp && vi main.cpp && mpic++ main.cpp -o main
mpic++ -fopenmp main.cpp -o main
mpirun -np 4 ./main
mpirun -host 192.168.142.129,127.0.0.1 -np 8 ./main