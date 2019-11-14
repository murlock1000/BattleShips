.PHONY: server game ai

all : server game ai

server :
	g++ serverWin.cpp stdcomm.cpp -o server

game :
	g++ gameWin.cpp stdcomm.cpp -o game.exe

ai :
	g++ ai/ai_random.cpp -o ai/ai_random.exe
