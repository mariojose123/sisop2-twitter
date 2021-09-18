build_server:
	g++ src/server.cpp -pthread -std=c++11 -o server

build_client:
	g++ src/client.cpp -pthread -std=c++11 -o client


