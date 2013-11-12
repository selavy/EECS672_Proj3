// AffPoint.h -- 3D Affine points
// This is a subset version of the full cryph::AffPoint class
// available as a part of the full cryph source tree available from:
// http://people.eecs.ku.edu/~miller/cryph

#ifndef AFFPOINT_H
#define AFFPOINT_H
namespace cryph { class AffPoint; }

#include <iostream>

#include "AffVector.h"

namespace cryph
{

// indices for extracting coordinates from points

const int W = -1;
const int X =  0;
const int Y =  1;
const int Z =  2;

class AffPoint
{
public:
	AffPoint(); // point at origin
	AffPoint(double xx, double yy, double zz=0.0);
	AffPoint(const AffPoint& p);
	AffPoint(const double* p); // assumes x=p[0], y=p[1], z=p[2]
	AffPoint(const float* p); // assumes x=p[0], y=p[1], z=p[2]
	AffPoint(const AffVector& v);
	virtual ~AffPoint();

	AffPoint	operator=(const AffPoint& rhs);
	AffPoint	operator+=(const AffVector& rhs);
	AffPoint	operator+=(const AffPoint& rhs);
	AffPoint	operator-=(const AffVector& rhs);
	AffPoint	operator*=(double f);
	AffPoint	operator/=(double f);
	double		operator[](int index) const; // read-only indexing
					// see indexing constants above

	AffPoint	operator+(const AffPoint& p2) const
				{ return AffPoint(x + p2.x, y + p2.y, z + p2.z); }
	AffPoint	operator*(double f) const
				{ return AffPoint (f*x, f*y, f*z); }
	AffPoint	operator/(double f) const
				{ return AffPoint (x/f, y/f, z/f); }
	AffVector	operator-(const AffPoint& p2) const
				{ return AffVector(x-p2.x, y-p2.y, z-p2.z); }
	AffPoint	operator+(const AffVector& v2) const
				{ return AffPoint(x+v2[DX], y+v2[DY], z+v2[DZ]); }
	AffPoint	operator-(const AffVector& v2) const
				{ return AffPoint(x-v2[DX], y-v2[DY], z-v2[DZ]); }

	// ---------- General Methods
	// Among other things, the following two methods are useful for interfacing
	// with OpenGL, especially routines like glBufferData. For example, a caller
	// can allocate an array of an appropriate size, then loop over an array
	// of AffPoint instances, invoking the appropriate aCoords method below.
	// For example:
	//     float* buf = new float[3*NUM_POINTS];
	//     for (int i=0 ; i<NUM_POINTS ; i++)
	//         affPointArray[i].aCoords(buf, 3*i);
	//     glBufferData(GL_ARRAY_BUFFER, 3*NUM_POINTS*sizeof(float),
	//                  buf, GL_STATIC_DRAW);
	//     delete [] buf;
	double*				aCoords(double* coords, int offset=0) const;
	float*				aCoords(float* coords, int offset=0) const;

	bool				coincidentWith(const AffPoint& p) const;
	double				distanceSquaredTo(const AffPoint& p) const;
	double				distanceTo(const AffPoint& p) const;

	// ---------- General Class Methods
	static void			setCoincidenceTolerance(double tol);
	
    // ---------- Global constants

	// 1. Special Points
	static const AffPoint		origin;
	static const AffPoint		xAxisPoint;
	static const AffPoint		yAxisPoint;
	static const AffPoint		zAxisPoint;

	// While the actual coordinates are public, maximum benefit of usage of
	// this class is achieved when callers use strictly the methods above.
	double	x;
	double	y;
	double	z;

private:
	static double	sCoincidenceTol;
};
}

// Global functions
std::ostream& operator<<(std::ostream& os, const cryph::AffPoint& p);
std::istream& operator>>(std::istream& is, cryph::AffPoint& p);

static cryph::AffPoint operator*(double f, const cryph::AffPoint& p)
	{ return cryph::AffPoint(f*p.x, f*p.y, f*p.z); }

#endif
