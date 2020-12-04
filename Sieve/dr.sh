if [ -z "$1" ] 
then
echo "No input found! Enter N as an input (eg - bash dr.sh 1000000).";
else
mpicc seive.cpp -lm -lstdc++ -o seive;
mpirun -np 2 ./seive $1;
fi
