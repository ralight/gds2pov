COMPILE=gcc -Wall -DDEBUG

all : run

clean : clean_gds2pov

gds2pov : gds_types.o process_cfg.o gds_parse.o gds2pov.o
	$(COMPILE) -DLINUX -o gds2pov gds2pov.o gds_parse.o gds_types.o process_cfg.o -lm

gds_types.o : gds_types.c gds_types.h gds2pov.h
	$(COMPILE) -c -DLINUX -o gds_types.o gds_types.c

process_cfg.o : process_cfg.c process_cfg.h
	$(COMPILE) -c -DLINUX -o process_cfg.o process_cfg.c

gds_parse.o : gds_parse.c gds_parse.h gds2pov.h gds_globals.h gds_types.o process_cfg.o
	$(COMPILE) -c -DLINUX -o gds_parse.o gds_parse.c
	
gds2pov.o : gds2pov.c gds2pov.h gds_types.o gds_parse.o gds_globals.h process_cfg.o
	$(COMPILE) -c -DLINUX -o gds2pov.o gds2pov.c
	
clean_gds2pov :
	rm -f gds2pov
	rm -f *.o

run : gds2pov
	./gds2pov layers.gds layers.pov
	./gds2pov fullexample.gds fullexample.pov

test : run
	diff layers.pov layers_ref.pov
