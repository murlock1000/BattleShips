.PHONY: ai clean

CXX = g++
CXXFLAGS = -g 

objects = stdcomm.o dbconnector.o
libraries = -lmysqlclient
executables = game server

all : $(executables)

server : server.o $(objects)
	$(CXX) $(CXXFLAGS) -o server server.o $(objects) $(libraries)

game : game.o $(objects)
	$(CXX) $(CXXFLAGS) -o game game.o $(objects) $(libraries)

*.o :
	$(CXX) $(CXXFLAGS) -c -o %.o %.cpp
	
clean:
	-rm $(executables) $(patsubst %, %.o, $(executables)) $(objects)
