/*
 * File: gdsparse.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef _GDSPARSE_H_
#define _GDSPARSE_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_STDINT_H
#  include <stdint.h>
typedef uint16_t u_int16_t;
#endif

#include <vector>
using namespace std;

#include <sys/types.h>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsobject.h"
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
	std::string _libname;
	std::string _topcellname;

	int16_t _currentlayer;
	float _currentwidth;
	int16_t _currentpathtype;
	gds_element_type _currentelement;
	int16_t _currenttexttype;
	int16_t _currentpresentation;
	std::string _textstring;
	int16_t _currentstrans;
	float _currentangle;
	int16_t _currentdatatype;
	float _currentmag;
	float _currentbgnextn;
	float _currentendextn;

	std::string _sname;
	int16_t _arrayrows, _arraycols;
	float _units;
	float _angle;
	FILE *_iptr;
	FILE *_optr;
	class GDSProcess *_process;
	class GDSConfig	 *_config;
	
	struct _Boundary *Boundary;
	int16_t _recordlen;

	/* Output options */
	bool _allow_multiple_output;
	bool _output_children_first;
	bool _bounding_output;
	bool _use_outfile;
	bool _generate_process;

	/*
	** Both of these variables have fixed bounds because
	** they are not dependant on the GDS2 spec, not on the
	** file we are parsing.
	** There will never be more than 70 records.
	** The maximum layer and datatype are both defined as
	** 255, but but could be as high as 32,767 because of
	** the way they are stored (2 byte int). It might be worth
	** checking if they are greater than 255
	*/
	bool _unsupported[70];
	bool _layer_warning[256][256];

	long _PathElements;
	long _BoundaryElements;
	long _BoxElements;
	long _TextElements;
	long _SRefElements;
	long _ARefElements;
 
	vector<class GDSObject*> _Objects;
	class GDSObject *_CurrentObject;

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
	int32_t GetFourByteSignedInt();
	int16_t GetTwoByteSignedInt();
	char *GetAsciiString();

	void ReportUnsupported(std::string Name, enum RecordNumbers rn);
	
	bool ParseFile();
	struct _Boundary *GetBoundary();

	/* Abstract functions to be implemented be inheriting class */
	virtual void OutputHeader() = 0;
	virtual void OutputFooter() = 0;
	/* End abstract functions */

	void RecursiveOutput(class GDSObject *Object, FILE *optr, float offx, float offy, long *objectid);
	class GDSObject *GetObjectRef(std::string Name);
public:
	GDSParse (class GDSConfig *config, class GDSProcess *process, bool generate_process);
	virtual ~GDSParse ();

	void SetOutputOptions(bool bounding_output, bool use_outfile, bool allow_multiple_output, bool output_children_first);
	bool Parse(FILE *iptr);
	void Output(FILE *optr, std::string topcell);
	virtual class GDSObject *NewObject(std::string Name) = 0;
};

#endif
