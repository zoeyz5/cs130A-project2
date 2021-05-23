#Makefile

all: project2_first.out project2_second.out

project2_first.out: Dictionary.cpp hash24.cpp main1.cpp 
		g++ -o project2_first.out main1.cpp Dictionary.cpp hash24.cpp

project2_second.out: Dictionary.cpp hash24.cpp main2.cpp 
		g++ -o project2_second.out main2.cpp Dictionary.cpp hash24.cpp

