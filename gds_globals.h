#ifndef _GDS_GLOBALS_H
#define _GDS_GLOBALS_H

typedef enum{
	elBoundary,
	elPath,
	elSRef,
	elARef,
	elText
} element_type;

typedef enum{
	modeFindNames,
	modeRecordNames,
	modeParse
} parse_mode;

extern char *libname;
extern char *textstring;
extern short currentlayer;
extern float currentwidth;
extern short currentpathtype;
extern element_type currentelement;
extern short currenttexttype;
extern short currentpresentation;
extern char *sname;
extern short arrayrows, arraycols;
extern float units;
extern unsigned short currentstrans;
extern float currentangle;
extern parse_mode Mode;
extern char **StrNames;
extern char *CurrentStrName;
extern int StrCount;
extern int CurrentStr;

#endif
