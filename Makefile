# Define LINUX or SOLARIS as appropriate
#COMPILE=g++ -Wall 
COMPILE=g++ -Wall -pg -DLINUX
#COMPILE=g++ -Wall -pg -fprofile-arcs -ftest-coverage

all : run

clean : clean_gds2pov

gds2pov : process_cfg.o gdsparse.o gds2pov.o gdsobjects.o gdsobject.o config_cfg.o
	$(COMPILE) -o gds2pov gds2pov.o gdsparse.o process_cfg.o gdsobject.o gdsobjects.o config_cfg.o -lm 

config_cfg.o : config_cfg.cpp config_cfg.h
	$(COMPILE) -c -o config_cfg.o config_cfg.cpp

process_cfg.o : process_cfg.cpp process_cfg.h
	$(COMPILE) -c -o process_cfg.o process_cfg.cpp

gdsparse.o : gdsparse.cpp gdsparse.h gds2pov.h gds_globals.h process_cfg.o
	$(COMPILE) -c -o gdsparse.o gdsparse.cpp

gdsobject.o : gdsobject.cpp gdsobject.h
	$(COMPILE) -c -o gdsobject.o gdsobject.cpp
	
gdsobjects.o : gdsobjects.cpp gdsobjects.h gdsobject.o
	$(COMPILE) -c -o gdsobjects.o gdsobjects.cpp
	
gds2pov.o : gds2pov.cpp gds2pov.h gdsparse.o gds_globals.h process_cfg.o
	$(COMPILE) -c -o gds2pov.o gds2pov.cpp
	
clean_gds2pov :
	rm -f gds2pov
	rm -f *.o

run : gds2pov
	./gds2pov layers.gds layers.pov -c config.txt -p process.txt
	./gds2pov fullexample.gds fullexample.pov -c config.txt -p process.txt

test : run
	diff layers.pov layers_ref.pov
