.PHONY: server game ai

all : server game ai

server :
	g++ server.cpp stdcomm-linux.cpp dbconnector/dbconnector.cpp -o server -lmysqlclient

game :
	g++ game.cpp stdcomm-linux.cpp dbconnector/dbconnector.cpp -o game.exe -lmysqlclient

ai :
	g++ ai/ai_random.cpp -o ai/ai_random.exe
