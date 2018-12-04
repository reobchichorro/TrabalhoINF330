all: leitura	

leitura:
	g++ -o leitura leitura.cpp -ljsoncpp -std=c++11
