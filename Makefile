GDS2POV_VERSION=0.8
DISTLOC=release
DISTDIR=gds2pov_${GDS2POV_VERSION}
#COMPILE=g++ -Wall -pg -fprofile-arcs -ftest-coverage
COMPILE=g++ -Wall
#COMPILE=g++ -Wall -pg -fprofile-arcs -ftest-coverage -DOGLVIEWER
OPENGL_LIB=-lGLU -lGL -lX11 -L/usr/X11R6/lib64/

all : run

help : targets

targets:
	echo "All gds2pov clean clean_profile run test bench"

clean : clean_gds2pov clean_profile

gds2pov : process_cfg.o gdsparse.o gdsparse_3ds.o gdsparse_ogl.o gds2pov.o gdsobjects.o gdsobject.o gdsobject_3ds.o gdsobject_pov.o gdsobject_ogl.o config_cfg.o gds_globals.o gdstext.o gdspath.o gdspolygon.o
	$(COMPILE) -o gds2pov gds2pov.o gdsparse.o gdsparse_3ds.o gdsparse_ogl.o process_cfg.o gdsobject.o gdsobject_3ds.o gdsobject_pov.o gdsobject_ogl.o gdsobjects.o config_cfg.o gds_globals.o gdstext.o gdspath.o gdspolygon.o $(OPENGL_LIB) -lm 

config_cfg.o : config_cfg.cpp config_cfg.h
	$(COMPILE) -c -o config_cfg.o config_cfg.cpp

process_cfg.o : process_cfg.cpp process_cfg.h
	$(COMPILE) -c -o process_cfg.o process_cfg.cpp

gdsparse.o : gdsparse.cpp gdsparse.h gds2pov.h gds_globals.h
	$(COMPILE) -c -o gdsparse.o gdsparse.cpp

gdsparse_3ds.o : gdsparse_3ds.cpp gdsparse.h
	$(COMPILE) -c -o gdsparse_3ds.o gdsparse_3ds.cpp

gdsparse_ogl.o : gdsparse_ogl.cpp gdsparse.h
	$(COMPILE) -c -o gdsparse_ogl.o gdsparse_ogl.cpp

gdsobject.o : gdsobject.cpp gdsobject.h
	$(COMPILE) -c -o gdsobject.o gdsobject.cpp
	
gdsobject_3ds.o : gdsobject_3ds.cpp gdsobject.h
	$(COMPILE) -c -o gdsobject_3ds.o gdsobject_3ds.cpp
	
gdsobject_ogl.o : gdsobject_ogl.cpp gdsobject.h
	$(COMPILE) -c -o gdsobject_ogl.o gdsobject_ogl.cpp
	
gdsobject_pov.o : gdsobject_pov.cpp gdsobject.h
	$(COMPILE) -c -o gdsobject_pov.o gdsobject_pov.cpp

gdstext.o : gdstext.cpp gdstext.h gdsobject.h
	$(COMPILE) -c -o gdstext.o gdstext.cpp

gdspath.o : gdspath.cpp gdspath.h gdsobject.h
	$(COMPILE) -c -o gdspath.o gdspath.cpp

gdspolygon.o : gdspolygon.cpp gdspolygon.h gdsobject.h
	$(COMPILE) -c -o gdspolygon.o gdspolygon.cpp

gdsobjects.o : gdsobjects.cpp gdsobjects.h
	$(COMPILE) -c -o gdsobjects.o gdsobjects.cpp
	
gds2pov.o : gds2pov.cpp gds2pov.h gdsparse.o gds_globals.h
	$(COMPILE) -c -o gds2pov.o gds2pov.cpp
	
gds_globals.o : gds_globals.h gds_globals.cpp
	$(COMPILE) -c -o gds_globals.o gds_globals.cpp

clean_gds2pov :
	rm -f gds2pov
	rm -f *.o

clean_profile :
	rm -f *.bb *.bbg *.da
	rm -f *.gcda *.gcno
	rm -f *.gcov

run : gds2pov
	#./gds2pov files/layers.gds files/layers.pov 
	#./gds2pov files/example.gds files/example.pov -c example_config.txt -p example_process.txt
	##./gds2pov files/layers.gds files/layers.pov -c config.txt -p process.txt
	#./gds2pov files/fullexample.gds files/fullexample.3ds -c config.txt -p process.txt -q
	./gds2pov files/boundary.gds files/boundary.pov -c config.txt -p process.txt -d -f 3ds
	./gds2pov files/rect.gds files/rect.pov -c config.txt -p process.txt  -d -f 3ds
	#./gds2pov files/layers.gds files/layers.pov -c config.txt -p process.txt -f ogl
	./gds2pov fullexample.gds layers.pov -c config.txt -p process.txt -f ogl -q -m wire

test : run
	diff files/layers.pov files/layers_ref.pov

bench : gds2pov
	./gds2pov files/dh_adc.gds files/dh_adc.pov
	gprof ./gds2pov | more

dist : gds2pov
	HERE=$(pwd)
	strip gds2pov
	mkdir -p ${DISTLOC}/
	cd ${DISTLOC}/
	mkdir -p ${DISTDIR}/
	cp -f ${HERE}/gds2pov ${DISTDIR}/
	cp -f ${HERE}/example_2D.png ${DISTDIR}/
	cp -f ${HERE}/example_3D.png ${DISTDIR}/
	cp -f ${HERE}/changes.txt ${DISTDIR}/
	cp -f ${HERE}/example.gds ${DISTDIR}/
	cp -f ${HERE}/example_config.txt ${DISTDIR}/
	cp -f ${HERE}/example_process.txt ${DISTDIR}/
	cp -f ${HERE}/readme.txt ${DISTDIR}/
	tar -cf ${DISTDIR}.tar ${DISTDIR}
	gzip ${DISTDIR}.tar
	cd ${HERE}/

destroy : gds2pov
	./gds2pov /dev/random files/random.pov -c config.txt -p process.txt

