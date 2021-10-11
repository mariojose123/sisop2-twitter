build:
	sudo apt-get install libboost-dev -y
	g++ src/server.cpp src/Profile.cpp src/packet.cpp src/DataTwitter.cpp  -o server  -lpthread -g -std=c++11
	g++ src/client.cpp  src/ClientCommunicationManager.cpp src/Profile.cpp src/packet.cpp src/DataTwitter.cpp  -o client -lpthread -g -std=c++11


build_server:
	g++ src/server.cpp src/Profile.cpp src/packet.cpp src/DataTwitter.cpp  -o server  -lpthread -g -std=c++11

build_client:
	g++ src/client.cpp  src/ClientCommunicationManager.cpp src/Profile.cpp src/packet.cpp src/DataTwitter.cpp  -o client -lpthread -g -std=c++11

create_client:
	./client $(user) 127.0.0.1 4934

debug_server:
	gdb




