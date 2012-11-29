all: client

client: async_client.o
	g++ -o client -lboost_system -lboost_thread -lpthread async_client.o

async_client.o: async_client.cpp
	g++ -o async_client.o -c async_client.cpp

client.o: client.cpp
	g++ -o client.o -c client.cpp
