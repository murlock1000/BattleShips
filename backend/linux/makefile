.PHONY: clean ai install

CXX = g++
CXXFLAGS = -g 

objs = stdcomm.o dbconnector.o
libflags = -lmysqlclient
execs = server game
ai_execs = ai_random register-ai

install = install
bindir = /usr/local/bin
datadir = /usr/local/share/battleships

all : $(execs) $(ai_execs)

server : server.o $(objs)
	$(CXX) $(CXXFLAGS) -o server server.o $(objs) $(libflags)

game : game.o $(objs)
	$(CXX) $(CXXFLAGS) -o game game.o $(objs) $(libflags)

ai_random : ai/ai_random.o
	$(CXX) $(CXXFLAGS) -o ai_random ai/ai_random.o

register-ai : ai/register-ai.o dbconnector.o
	$(CXX) $(CXXFLAGS) -o register-ai ai/register-ai.o dbconnector.o $(libflags)

*.o :
	$(CXX) $(CXXFLAGS) -c -o %.o %.cpp

install : all
	mkdir -p $(bindir)
	mkdir -p $(datadir)/ai
	for exec in $(execs); do \
		$(install) -m755 $$exec $(bindir)/battleships-$$exec; \
	done
	for exec in $(ai_execs); do \
		$(install) -m755 $$exec $(datadir)/ai; \
	done
	install -m755 register-ai $(bindir)/battleships-register-ai
	install -m644 ai/kaip_sukurti_ai $(datadir)/ai

uninstall :
	-rm -r $(datadir) \
	$(patsubst %, $(bindir)/battleships-%, $(execs)) \
	$(bindir)/battleships-register-ai
	
clean :
	-rm $(execs) $(patsubst %, %.o, $(execs)) $(ai_execs) \
	   	$(patsubst %, ai/%.o, $(ai_execs))  $(objs)
