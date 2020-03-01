.PHONY: clean ai install

CXX = g++
CXXFLAGS = -g -Wno-narrowing

objs = stdcomm.o dbconnector.o server.o game.o register-ai.o  main.o
libflags = -lmysqlclient
execs = battleships-server

install = install
bindir = /usr/local/bin
datadir = /usr/local/share/battleships

all : $(execs)

battleships-server : $(objs)
	$(CXX) $(CXXFLAGS) -o battleships-server $(objs) $(libflags)

*.o :
	$(CXX) $(CXXFLAGS) -c -o %.o %.cpp

install : all
	mkdir -p $(bindir)
	mkdir -p $(datadir)/ai
	for exec in $(execs); do \
		$(install) -m755 $$exec $(bindir); \
	done
	install -m644 ai/kaip_sukurti_ai $(datadir)/ai

uninstall :
	-rm -r $(datadir) \
	$(patsubst %, $(bindir)/%, $(execs))
	
clean :
	-rm $(execs) $(objs)
