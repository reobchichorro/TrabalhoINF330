all: trabalho	

trabalho:
	g++ -o trabalho trabalho.cpp -ljsoncpp -std=c++11

clear:
	rm trabalho