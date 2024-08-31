rm main.cpp && vi main.cpp && mpic++ main.cpp -o main
mpic++ -fopenmp main.cpp -o main
mpirun -np 4 ./main
mpirun -host 192.168.142.129,192.168.142.128 -np 8 ./main

rm main.cpp && vi main.cpp && mpic++ main.cpp -o main
ulimit -s unlimited
rm -f main.cpp && vi main.cpp && mpic++ -fopenmp main.cpp -o main

ssh 192.168.142.129