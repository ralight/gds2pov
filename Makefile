COMPILE=gcc -Wall

all : gds2pov

clean : clean_gds2pov

gds2pov : 
	$(COMPILE) -DLINUX -o gds2pov gds2pov.c gds_parse.c gds_types.c process_cfg.c -lm

gds_types.o : gds2pov.h
	$(COMPILE) -DLINUX -o gds_types.o gds_types.c

gds2pov.o : gds2pov.c gsd2pov.h gds_types.
	
clean_gds2pov :
	rm -f memuse
