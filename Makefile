all: sequential pthread openmp opencilk

sequential: sequential.cpp helper_functions.cpp containers.cpp
	g++ -O2 -o sequential sequential.cpp helper_functions.cpp containers.cpp -I./include
	
pthread: pthread.cpp helper_functions.cpp containers.cpp
	g++ -O2 -o pthread pthread.cpp helper_functions.cpp containers.cpp -lpthread -I./include
	
openmp:  openmp.cpp helper_functions.cpp containers.cpp
	g++ -O2 -o openmp pthread.cpp helper_functions.cpp containers.cpp -fopenmp -I./include
	
opencilk:  opencilk.cpp helper_functions.cpp containers.cpp
	~/Downloads/OpenCilk-10.0.1-Linux/bin/clang++ -O2 -o opencilk pthread.cpp helper_functions.cpp containers.cpp -fopencilk -I./include
	 
	 
	 
