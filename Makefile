all: client

client: client.o main.o
	g++ -o client -lboost_system -lboost_thread -lpthread client.o main.o

main.o: main.cpp client.cpp
	g++ -o main.o -c main.cpp

client.o: client.cpp
	g++ -o client.o -c client.cpp
