/*
 * File: gdsparse.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _GDSPARSE_H_
#define _GDSPARSE_H_

#include <stdint.h>

#include <vector>
using namespace std;

#include <sys/types.h>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsobject.h"

/*
 * We need byte swapping functions.
 * If these exist for a particular platform, use the (presumably) optimised
 * versions provided. Otherwise, use the network functions htonl() and htons().
 */

#if defined(linux)
#	include <endian.h>
#	include <byteswap.h>
#	define endian_swap_long(A) bswap_32((A))
#	define endian_swap_short(A) bswap_16((A))
#elif defined(WIN32)
//#	define __LITTLE_ENDIAN 1234
//#	define __BIG_ENDIAN 4321
//#	define __BYTE_ORDER __LITTLE_ENDIAN
//	long endian_swap_long(long value);
//	short endian_swap_short(short value);
#ifdef HAVE_WINSOCK2_H
#	include <winsock2.h>
#	define endian_swap_long(A) htonl((A))
#	define endian_swap_short(A) htons((A))
#endif
#else
	/* Generic includes for Unix-alike systems */
	/* Includes at least Solaris, FreeBSD, HP-UX */
#	include <arpa/nameser_compat.h>
#	include <netinet/in.h>
#	define endian_swap_long(A) htonl((A))
#	define endian_swap_short(A) htons((A))
#endif

class GDSParse_pov;
class GDSParse_svg;

class GDSParse
{
protected:
	std::string m_libname;
	std::string m_topcellname;

	int16_t m_currentlayer;
	int16_t m_currentdatatype;
	float m_currentwidth;
	int16_t m_currentpathtype;
	gds_element_type m_currentelement;
	int16_t m_currenttexttype;
	int16_t m_currentpresentation;
	std::string m_textstring;
	int16_t m_currentstrans;
	float m_currentangle;
	float m_currentmag;
	float m_currentbgnextn;
	float m_currentendextn;

	std::string m_sname;
	int16_t m_arrayrows, m_arraycols;
	float m_units;
	float m_angle;
	FILE *m_iptr;
	FILE *m_optr;
	GDSProcess *m_process;
	GDSConfig	 *m_config;

	struct _Boundary *m_boundary;
	int16_t m_recordlen;

	/* Output options */
	bool m_bounding_output;
	bool m_use_outfile;
	bool m_allow_multiple_output;
	bool m_output_children_first;
	bool m_generate_process;

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
	bool m_unsupported[70];
	bool m_layer_warning[256][256];

	long m_pathelements;
	long m_boundaryelements;
	long m_boxelements;
	long m_textelements;
	long m_srefelements;
	long m_arefelements;

	vector<GDSObject*> m_objects;
	GDSObject *m_currentobject;

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

	void ReportUnsupported(std::string name, enum RecordNumbers rn);

	bool ParseFile();
	void AssignASRefs(void);
	struct _Boundary *GetBoundary();

	virtual void OutputHeader() { };
	virtual void OutputFooter() { };

	void RecursiveOutput(GDSObject *object, FILE *optr, float offx, float offy, long *objectid);
	GDSObject *GetObjectRef(std::string name);
public:
	GDSParse() { };
	GDSParse(GDSConfig *config, GDSProcess *process, bool generate_process);
	virtual ~GDSParse ();

	bool Parse(FILE *iptr);
	void Output(FILE *optr, std::string topcell);
	virtual GDSObject *NewObject(std::string name) { return new GDSObject(name); };

	friend class GDSParse_pov;
	friend class GDSParse_svg;
};

#endif
