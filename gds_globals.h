#ifndef _GDS_GLOBALS_H
#define _GDS_GLOBALS_H

typedef enum{
	elBoundary,
	elPath,
	elSRef,
	elARef
} element_type;

extern char *libname;
extern short currentlayer;
extern float currentwidth;
extern short currentpathtype;
extern element_type currentelement;
extern short currenttexttype;
extern short currentpresentation;
extern char *sname;
extern short arrayrows, arraycols;
extern float units;

#endif
