#ifndef __GDSPARSE_H__
#define __GDSPARSE_H__

#include <stdarg.h>
#include <string>
using namespace std;

#ifdef LINUX
#include <endian.h>
#include <byteswap.h>
#define endian_swap_long(A) bswap_32((A))
#define endian_swap_short(A) bswap_16((A))
#endif

#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsobject.h"
#include "gdsobjects.h"

#ifdef WIN32
#include "gds_types.h"
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

typedef enum{
	cpCentre,
	cpBottomLeft,
	cpBottomRight,
	cpTopLeft,
	cpTopRight
} CameraPosition;

class GDSParse
{
private:
	short gdsversion;
	char *libname;

	short currentlayer;
	float currentwidth;
	short currentpathtype;
	gds_element_type currentelement;
	short currenttexttype;
	short currentpresentation;
	char *textstring;
	unsigned short currentstrans;
	float currentangle;
	short currentdatatype;

	char *sname;
	short arrayrows, arraycols;
	float units;
	float angle;
	int mirror;
	FILE *iptr;
	FILE *optr;
	class GDSProcess *process;
	
	short recordlen;

	long PathElements;
	long BoundaryElements;
	long TextElements;
	long SRefElements;
	long ARefElements;

	class GDSObjects *Objects;
	class GDSObject *CurrentObject;

	/* gds_parse.h functions */
	void ParseHeader();
	void ParseLibName();
	void ParseSName();
	void ParseUnits();
	void ParseStrName();
	void ParseXY();
	void ParseXYPath();
	void ParseXYBoundary();
	void ParseSTrans();

	void Parse();
	void HandleSRef();
	void HandleARef();
	void HandleBoundary();
	void HandlePath();

	short GetBitArray();
	double GetEightByteReal();
	long GetFourByteSignedInt();
	short GetTwoByteSignedInt();
	char *GetAsciiString();
	
	void OutputPOVHeader();
public:
	GDSParse (char *infile, char *outfile, char *processfile);
	~GDSParse ();
};

#endif // __GDSPARSE_H__

