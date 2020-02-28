.PHONY: clean install

CXX = g++
CXXFLAGS = -g 

objects = stdcomm.o dbconnector.o
libraries = -lmysqlclient
executables = server game

install = install
install-dir = /usr/local

all : $(executables)

server : server.o $(objects)
	$(CXX) $(CXXFLAGS) -o server server.o $(objects) $(libraries)

game : game.o $(objects)
	$(CXX) $(CXXFLAGS) -o game game.o $(objects) $(libraries)

*.o :
	$(CXX) $(CXXFLAGS) -c -o %.o %.cpp

install : all
	for exec in $(executables); do \
		$(install) -m755 $$exec $(install-dir)/bin/battleships-$$exec; \
	done
	
clean :
	-rm $(executables) $(patsubst %, %.o, $(executables)) $(objects)
