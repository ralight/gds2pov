#ifndef __GDSPARSE_H__
#define __GDSPARSE_H__

#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsobject.h"
#include "gdsobjects.h"
#include "gds_types.h"

typedef enum{
	cpCentre,
	cpBottomLeft,
	cpBottomRight,
	cpTopLeft,
	cpTopRight
} CameraPosition;

class GDSParse
{
protected:
	char			*_libname;
	char			*_topcellname;

	short			_currentlayer;
	float			_currentwidth;
	short			_currentpathtype;
	gds_element_type	_currentelement;
	short			_currenttexttype;
	short			_currentpresentation;
	char			*_textstring;
	unsigned short		_currentstrans;
	float			_currentangle;
	short			_currentdatatype;
	float			_currentmag;
	float			_currentbgnextn;
	float			_currentendextn;

	char			*_sname;
	short			_arrayrows, _arraycols;
	float			_units;
	float			_angle;
	FILE			*_iptr;
	FILE			*_optr;
	class GDSProcess	*_process;
	class GDSConfig		*_config;
	
	short			_recordlen;

	bool			_unsupported[70];

	long			_PathElements;
	long			_BoundaryElements;
	long			_TextElements;
	long			_SRefElements;
	long			_ARefElements;

	class GDSObjects	*_Objects;
	class GDSObject		*_CurrentObject;

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

	void HandleSRef();
	void HandleARef();
	void HandleBoundary();
	void HandlePath();

	short GetBitArray();
	double GetEightByteReal();
	long GetFourByteSignedInt();
	short GetTwoByteSignedInt();
	char *GetAsciiString();

	void ReportUnsupported(char *Name, enum RecordNumbers rn);
	
	bool ParseFile();

	/* Abstract functions to be implemented be inheriting class */
	virtual void OutputHeader() = 0;
	virtual void OutputFooter() = 0;
	/* End abstract functions */

	void RecursiveOutput(class GDSObject *Object, FILE *optr, float offx, float offy, long *objectid, bool allow_multiple_output);
public:
	GDSParse (class GDSConfig *config, class GDSProcess *process);
	virtual ~GDSParse ();

	bool Parse(FILE *iptr);
	void Output(FILE *optr, char *topcell, bool use_outfile, bool allow_multiple_output, bool bounding_output);
	virtual class GDSObject *NewObject(char *Name) = 0;
};

#endif // __GDSPARSE_H__
