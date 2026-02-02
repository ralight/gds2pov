/*
 * File: gdspath.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the GDSPath class which is used to represent the GDS path type.
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

#include <math.h>

#include "gdsobject.h"
#include "gdspath.h"

namespace GDS2X {

Path::Path(int type, double width,
				double bgnextn, double endextn,
				ProcessLayer *layer) :
				m_type(type), m_width(width),
				m_bgnextn(bgnextn), m_endextn(endextn), m_layer(layer)
{
}

Path::~Path()
{
}

void Path::AddPoint(double x, double y)
{
	m_coords.push_back(Point(x, y));
}


void Path::SetRotation(double x, double y, double z)
{
	m_rotate.x = x;
	m_rotate.y = y;
	m_rotate.z = z;
}

double Path::GetXCoords(unsigned int index)
{
	if(index < m_coords.size()){
		return m_coords[index].x;
	}
	return 0.0;
}

double Path::GetYCoords(unsigned int index)
{
	if(index < m_coords.size()){
		return m_coords[index].y;
	}
	return 0.0;
}

unsigned int Path::GetPoints()
{
	return m_coords.size();
}

double Path::GetHeight()
{
	return m_layer->GetHeight();
}

double Path::GetThickness()
{
	return m_layer->GetThickness();
}

double Path::GetWidth()
{
	return m_width;
}

double Path::GetBgnExtn()
{
	return m_bgnextn;
}

double Path::GetEndExtn()
{
	return m_endextn;
}

int Path::GetType()
{
	return m_type;
}

ProcessLayer *Path::GetLayer()
{
	return m_layer;
}


int Path::GetPointCentre(unsigned int idx, double &x, double &y)
{
	if(idx < m_coords.size()){
		x = m_coords[idx].x;
		y = m_coords[idx].y;
		return 1;
	}else{
		return 0;
	}
}

int Path::GetPoint2D(unsigned int idx, double &x, double &y)
{
	if(this->GetWidth()){
		double BgnExtn;
		double EndExtn;

		switch(this->GetType()){
			case 1:
			case 2:
				BgnExtn = this->GetWidth(); /* Width has already been scaled to half */
				EndExtn = this->GetWidth();
					break;
			case 4:
				BgnExtn = this->GetBgnExtn();
				EndExtn = this->GetEndExtn();
				break;
			default:
				BgnExtn = 0.0;
				EndExtn = 0.0;
				break;
		}

		if(idx >= this->GetPoints()*2){
			return 0;
		}
		unsigned int point = idx/2;

		{
			double XCoords_j2;
			double YCoords_j2;
			double angle12 = 0.0, angle23 = 0.0;
			double angleX, angleY;
			double PathWidth = this->GetWidth();
			double extn_x, extn_y;

			XCoords_j2 = this->GetXCoords(point);
			YCoords_j2 = this->GetYCoords(point);
			if(point>0) {
				double XCoords_j1 = this->GetXCoords(point-1);
				double YCoords_j1 = this->GetYCoords(point-1);
				angle12 = atan2(XCoords_j1 - XCoords_j2, YCoords_j2 - YCoords_j1);
			}
			if(point+1<this->GetPoints()) {
				double XCoords_j3 = this->GetXCoords(point+1);
				double YCoords_j3 = this->GetYCoords(point+1);
				angle23 = atan2(XCoords_j2 - XCoords_j3, YCoords_j3 - YCoords_j2);
			}

			if(point == 0) {
				extn_x = BgnExtn * sin(angle23);
				extn_y = BgnExtn * cos(angle23);
				angleX = cos(angle23);
				angleY = sin(angle23);
			}else if(point == this->GetPoints()-1) {
				extn_x = EndExtn * -sin(angle12);
				extn_y = EndExtn * -cos(angle12);
				angleX = cos(angle12);
				angleY = sin(angle12);
			}else{
				extn_x = 0.0;
				extn_y = 0.0;
				double ax = sin(angle12)*tan((angle23-angle12)*.5);
				double ay = cos(angle12)*tan((angle23-angle12)*.5);
				if(ax > 1.0 || ay > 1.0){
					printf("WARNING: Path angle is acute and will not be properly represented.\n");
				}
				angleX = cos(angle12) - ax;
				angleY = sin(angle12) + ay;
			}

			switch(idx%2){
				case 0:
					x = XCoords_j2 + PathWidth * angleX + extn_x;
					y = YCoords_j2 + PathWidth * angleY - extn_y;
					return 1;

				case 1:
					x = XCoords_j2 - PathWidth * angleX + extn_x;
					y = YCoords_j2 - PathWidth * angleY - extn_y;
					return 1;

				default:
					return 0;
			}
		}
	}

	return 0;
}


/* FIXME - use GetPoint2D here, instead of duplicating code.
 * This will result in point ordering changing, and hence also face vertices
 */
int Path::GetPoint3D(unsigned int idx, double &x, double &y, double &z)
{
	if(this->GetWidth()){
		double BgnExtn;
		double EndExtn;

		switch(this->GetType()){
			case 1:
			case 2:
				BgnExtn = this->GetWidth(); /* Width has already been scaled to half */
				EndExtn = this->GetWidth();
					break;
			case 4:
				BgnExtn = this->GetBgnExtn();
				EndExtn = this->GetEndExtn();
				break;
			default:
				BgnExtn = 0.0;
				EndExtn = 0.0;
				break;
		}

		if(idx >= this->GetPoints()*4){
			return 0;
		}
		unsigned int point = idx/4;

		{
			double XCoords_j2;
			double YCoords_j2;
			double angle12 = 0.0, angle23 = 0.0;
			double angleX, angleY;
			double PathWidth = this->GetWidth();
			double extn_x, extn_y;

			XCoords_j2 = this->GetXCoords(point);
			YCoords_j2 = this->GetYCoords(point);
			if(point>0) {
				double XCoords_j1 = this->GetXCoords(point-1);
				double YCoords_j1 = this->GetYCoords(point-1);
				angle12 = atan2(XCoords_j1 - XCoords_j2, YCoords_j2 - YCoords_j1);
			}
			if(point+1<this->GetPoints()) {
				double XCoords_j3 = this->GetXCoords(point+1);
				double YCoords_j3 = this->GetYCoords(point+1);
				angle23 = atan2(XCoords_j2 - XCoords_j3, YCoords_j3 - YCoords_j2);
			}

			if(point == 0) {
				extn_x = BgnExtn * sin(angle23);
				extn_y = BgnExtn * cos(angle23);
				angleX = cos(angle23);
				angleY = sin(angle23);
			}else if(point == this->GetPoints()-1) {
				extn_x = EndExtn * -sin(angle12);
				extn_y = EndExtn * -cos(angle12);
				angleX = cos(angle12);
				angleY = sin(angle12);
			}else{
				extn_x = 0.0;
				extn_y = 0.0;
				double ax = sin(angle12)*tan((angle23-angle12)*.5);
				double ay = cos(angle12)*tan((angle23-angle12)*.5);
				if(ax > 1.0 || ay > 1.0){
					printf("WARNING: Path angle is acute and will not be properly represented.\n");
				}
				angleX = cos(angle12) - ax;
				angleY = sin(angle12) + ay;
			}

			switch(idx%4){
				case 0:
					x = XCoords_j2 + PathWidth * angleX + extn_x;
					y = YCoords_j2 + PathWidth * angleY - extn_y;
					z = this->GetHeight() + this->GetThickness();
					return 1;

				case 1:
					x = XCoords_j2 - PathWidth * angleX + extn_x;
					y = YCoords_j2 - PathWidth * angleY - extn_y;
					z = this->GetHeight() + this->GetThickness();
					return 1;

				case 2:
					x = XCoords_j2 + PathWidth * angleX + extn_x;
					y = YCoords_j2 + PathWidth * angleY - extn_y;
					z = this->GetHeight();
					return 1;

				case 3:
					x = XCoords_j2 - PathWidth * angleX + extn_x;
					y = YCoords_j2 - PathWidth * angleY - extn_y;
					z = this->GetHeight();
					return 1;

				default:
					return 0;
			}
		}
	}

	return 0;
}


int Path::GetFace3D(unsigned int idx, int &v1, int &v2, int &v3)
{
	unsigned int PathPoints = this->GetPoints();
	if(PathPoints == 0 || idx >= (PathPoints-1)*8+4){
		return 0;
	}

	switch(idx){
		case 0:
			v1 = 1;
			v2 = 0;
			v3 = 2;
			return 1;
		case 1:
			v1 = 2;
			v2 = 3;
			v3 = 1;
			return 1;
		case 2:
			v1 = 1+4*PathPoints-4;
			v2 = 2+4*PathPoints-4;
			v3 = 0+4*PathPoints-4;
			return 1;
		case 3:
			v1 = 2+4*PathPoints-4;
			v2 = 1+4*PathPoints-4;
			v3 = 3+4*PathPoints-4;
			return 1;
		default:
			break;
	}

	idx -= 4;
	unsigned int point = idx / 8;

	switch(idx % 8){
		case 0:
			v1 = 1+4*point;
			v2 = 5+4*point;
			v3 = 0+4*point;
			return 1;
		case 1:
			v1 = 4+4*point;
			v2 = 0+4*point;
			v3 = 5+4*point;
			return 1;
		case 2:
			v1 = 3+4*point;
			v2 = 2+4*point;
			v3 = 6+4*point;
			return 1;
		case 3:
			v1 = 6+4*point;
			v2 = 7+4*point;
			v3 = 3+4*point;
			return 1;
		case 4:
			v1 = 3+4*point;
			v2 = 7+4*point;
			v3 = 1+4*point;
			return 1;
		case 5:
			v1 = 5+4*point;
			v2 = 1+4*point;
			v3 = 7+4*point;
			return 1;
		case 6:
			v1 = 2+4*point;
			v2 = 0+4*point;
			v3 = 4+4*point;
			return 1;
		case 7:
			v1 = 4+4*point;
			v2 = 6+4*point;
			v3 = 2+4*point;
			return 1;
		default:
			return 0;
	}
}

}
