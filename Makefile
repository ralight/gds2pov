# Define LINUX or SOLARIS as appropriate
#COMPILE=g++ -Wall 
COMPILE=g++ -Wall -DLINUX -O2 
#COMPILE=g++ -Wall -pg -fprofile-arcs -ftest-coverage -DLINUX

all : run

help : targets

targets:
	echo "All gds2pov clean clean_profile run test bench"

clean : clean_gds2pov

gds2pov : .process_cfg.o .gdsparse.o .gds2pov.o .gdsobjects.o .gdsobject.o .config_cfg.o .gds_globals.o
	$(COMPILE) -o gds2pov .gds2pov.o .gdsparse.o .process_cfg.o .gdsobject.o .gdsobjects.o .config_cfg.o .gds_globals.o -lm 

.config_cfg.o : config_cfg.cpp config_cfg.h
	$(COMPILE) -c -o .config_cfg.o config_cfg.cpp

.process_cfg.o : process_cfg.cpp process_cfg.h
	$(COMPILE) -c -o .process_cfg.o process_cfg.cpp

.gdsparse.o : gdsparse.cpp gdsparse.h gds2pov.h gds_globals.h
	$(COMPILE) -c -o .gdsparse.o gdsparse.cpp

.gdsobject.o : gdsobject.cpp gdsobject.h
	$(COMPILE) -c -o .gdsobject.o gdsobject.cpp
	
.gdsobjects.o : gdsobjects.cpp gdsobjects.h .gdsobject.o
	$(COMPILE) -c -o .gdsobjects.o gdsobjects.cpp
	
.gds2pov.o : gds2pov.cpp gds2pov.h .gdsparse.o gds_globals.h
	$(COMPILE) -c -o .gds2pov.o gds2pov.cpp
	
.gds_globals.o : gds_globals.h gds_globals.cpp
	$(COMPILE) -c -o .gds_globals.o gds_globals.cpp

clean_gds2pov :
	rm -f gds2pov
	rm -f .*.o

clean_profile :
	rm -f *.bb *.bbg *.da

run : gds2pov
	./gds2pov layers.gds layers.pov 
	#./gds2pov layers.gds layers.pov -c config.txt -p process.txt
	#./gds2pov fullexample.gds fullexample.pov -c config.txt -p process.txt

test : run
	diff layers.pov layers_ref.pov

bench : gds2pov
	./gds2pov dh_adc.gds dh_adc.pov
	gprof ./gds2pov | more

dist : gds2pov
	strip gds2pov
	mkdir -p dist/gds2pov
	cp -f gds2pov dist/gds2pov/
	cp -f example_2D.png dist/gds2pov/
	cp -f example_3D.png dist/gds2pov/
	cp -f changes.txt dist/gds2pov/
	cp -f example.gds dist/gds2pov/
	cp -f example_config.txt dist/gds2pov/
	cp -f example_process.txt dist/gds2pov/
	cp -f readme.txt dist/gds2pov/
