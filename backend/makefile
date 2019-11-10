.PHONY: server game ai

all : server game ai

server :
	g++ server.cpp stdcomm-linux.cpp -o server

game :
	g++ game.cpp stdcomm-linux.cpp -o game.exe

ai :
	g++ ai/ai_random.cpp -o ai/ai_random.exe
