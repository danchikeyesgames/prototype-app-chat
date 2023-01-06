CC=g++
FLAGS=-std=c++99 -c

SERVERFILES=src/server/ServerSockets.cpp src/server/ServerApp.cpp src/SocketIp.cpp src/Message.cpp src/server/Server.cpp src/Threadpool.cpp
CLIENTFILES=src/client/ClientSockets.cpp src/client/ClientApp.cpp src/SocketIp.cpp src/Message.cpp src/client/Client.cpp

SERVEROBJECTS=obj/ServerSockets.o obj/ServerApp.o obj/SocketIp.o obj/Message.o obj/Server.o obj/Threadpool.o
CLIENTOBJECTS=obj/ClientSockets.o obj/ClientApp.o obj/SocketIp.o obj/Message.o obj/Client.o

build_client.elf: $(CLIENTOBJECTS) obj
	$(CC) $(CLIENTOBJECTS) -o build_client.elf

build_server.elf: $(SERVEROBJECTS) obj
	$(CC) $(SERVEROBJECTS) -o build_server.elf

# SERVER
obj/ServerSockets.o: obj src/server/ServerSockets.cpp 
	$(CC) $(FLAGS) src/server/ServerSockets.cpp -o obj/ServerSockets.o

obj/ServerApp.o: obj src/server/ServerApp.cpp
	$(CC) $(FLAGS) src/server/ServerApp.cpp -o obj/ServerApp.o

obj/Server.o: src/server/Server.cpp
	$(CC) $(FLAGS) src/server/Server.cpp -o obj/Server.o

# CLIENT
obj/ClientSockets.o: obj src/client/ClientSockets.cpp
	$(CC) $(FLAGS) src/client/ClientSockets.cpp -o obj/ClientSockets.o

obj/ClientApp.o: obj src/client/ClientApp.cpp
	$(CC) $(FLAGS) src/client/ClientApp.cpp -o obj/ClientApp.o

obj/Client.o: src/client/Client.cpp
	$(CC) $(FLAGS) src/client/Client.cpp -o obj/Client.o

obj/Threadpool.o: src/Threadpool.cpp
	$(CC) $(FLAGS) src/Threadpool.cpp -o obj/Threadpool.o

# COMMON
obj:
	mkdir obj

obj/Message.o: obj src/Message.cpp
	$(CC) $(FLAGS) src/Message.cpp -o obj/Message.o

obj/SocketIp.o: src/SocketIp.cpp
	$(CC) $(FLAGS) src/SocketIp.cpp -o obj/SocketIp.o

# CLEAR
clean: build_client.elf build_server.elf obj
	rm -rf obj build_client.elf build_server.elf