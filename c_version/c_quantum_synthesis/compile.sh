rm *.gch *.o
g++ -g -O3 -c -std=c++11 *.cpp *.h
g++ -g -O3 -o simp_cont_sizet *.o 
