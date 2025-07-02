/*
 * File: gdsparse.h
 * Author: Roger Light
 * Project: gds2x
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

#ifndef GDSPARSE_H
#define GDSPARSE_H

#include <stdint.h>

#include <unordered_map>

#include <sys/types.h>

#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsobject.h"

namespace GDS2X {

class Parse
{
private:
	void ParseXYARef(bool Flipped);
	void ParseXYSRef(bool Flipped);
	void ParseXYText(bool Flipped);
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
	Process *m_process;

	struct Boundary *m_boundary;
	int16_t m_recordlen;

	/* Output options */
	bool m_bounding_output;
	bool m_use_outfile;
	bool m_allow_multiple_output;
	bool m_output_children_first;
	bool m_generate_process;
	std::string m_outfile;
	std::string m_format;

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

	std::unordered_map<std::string, Object*> m_objects;
	Object *m_currentobject;

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

	int ParseFileInternal();
	void AssignASRefs(void);

	virtual void OutputHeader() { };
	virtual void OutputFooter() { };

	void RecursiveOutput(Object *object);
	Object *GetObjectRef(std::string name);
public:
	Parse() { };
	Parse(Process *process, bool generate_process);
	virtual ~Parse ();

	std::unordered_map<std::string, Object *> LoadMacroFile(std::string filename);
	struct Boundary *GetBoundary();
	bool ParseFile(FILE *iptr);
	void Output();
	virtual Object *NewObject(std::string name) { return new Object(name); };

	float GetUnits();
	Process *GetProcess();
	std::unordered_map<std::string, Object*> GetObjects();
};

}
#endif
