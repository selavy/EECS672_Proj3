// AffPoint.c++ -- 3D Affine points
// This is a subset version of the full cryph::AffPoint class
// available as a part of the full cryph source tree available from:
// http://people.eecs.ku.edu/~miller/cryph

#include <stdlib.h>

#include "AffPoint.h"
#include "AffVector.h"

namespace cryph
{

// ---------- Global constants

// 1. Special Points
const AffPoint AffPoint::origin     = AffPoint(0.0 , 0.0 , 0.0);
const AffPoint AffPoint::xAxisPoint = AffPoint(1.0 , 0.0 , 0.0);
const AffPoint AffPoint::yAxisPoint = AffPoint(0.0 , 1.0 , 0.0);
const AffPoint AffPoint::zAxisPoint = AffPoint(0.0 , 0.0 , 1.0);

// Class variables

double	AffPoint::sCoincidenceTol = 1.0e-5;

// --------- basic constructors
	
AffPoint::AffPoint() : x(0), y(0), z(0)
{
}

AffPoint::AffPoint(double xx, double yy, double zz) : x(xx), y(yy), z(zz)
{
}

AffPoint::AffPoint(const AffPoint& p) : x(p.x), y(p.y), z(p.z)
{
}

// --------- other constructors

AffPoint::AffPoint(const double* p) : x(p[0]), y(p[1]), z(p[2])
{
}

AffPoint::AffPoint(const float* p) : x(p[0]), y(p[1]), z(p[2])
{
}

AffPoint::AffPoint(const AffVector& v) : x(v[DX]), y(v[DY]), z(v[DZ])
{
}

// --------- destructor

AffPoint::~AffPoint()
{
}

// --------- General Class Methods

void
AffPoint::setCoincidenceTolerance(double tol)
{
	if (tol >= 0.0) // really better be strictly > 0!!!
		AffPoint::sCoincidenceTol = tol;
}

// --------- operators as methods

AffPoint
AffPoint::operator=(const AffPoint& rhs)
{
	x = rhs.x; y = rhs.y ; z = rhs.z;
	return *this;
}

AffPoint
AffPoint::operator+=(const AffVector& rhs)
{
	x += rhs[DX]; y += rhs[DY] ; z += rhs[DZ];
	return *this;
}

AffPoint
AffPoint::operator+=(const AffPoint& rhs)
{
	x += rhs.x; y += rhs.y ; z += rhs.z;
	return *this;
}

AffPoint
AffPoint::operator-=(const AffVector& rhs)
{
	x -= rhs[DX]; y -= rhs[DY] ; z -= rhs[DZ];
	return *this;
}

AffPoint
AffPoint::operator*=(double f)
{
	x *= f; y *= f ; z *= f;
	return *this;
}

AffPoint
AffPoint::operator/=(double f)
{
	x /= f; y /= f ; z /= f;
	return *this;
}
double
AffPoint::operator[](int index) const
{
	// read-only indexing

	switch (index)
	{
		case W:
			return 1.0; // w == 1
		case X:
			return x;
		case Y:
			return y;
		case Z:
			return z;
	}

	return 0.0;
}

// operators that are external functions, not methods

std::ostream&
operator<<(std::ostream& os, const AffPoint& p)
{
	os << '(' << p[X] << ", " << p[Y] << ", " << p[Z] << ')';
	return os;
}
std::istream&
operator>>(std::istream& is, AffPoint& p)
{
	double x, y, z;
	is >> x >> y >> z;
	p = AffPoint(x,y,z);
	return is;
}

// --------- General Instance Methods

double*
AffPoint::aCoords(double* coords, int offset) const
{
	// extract affine coords and place into a double precision array

	coords[offset] = x;
	coords[offset+1] = y;
	coords[offset+2] = z;
	return coords;
}

float*
AffPoint::aCoords(float* coords, int offset) const
{
	// extract affine coords and place into a single precision (float) array

	coords[offset] = static_cast<float>(x);
	coords[offset+1] = static_cast<float>(y);
	coords[offset+2] = static_cast<float>(z);
	return coords;
}

bool
AffPoint::coincidentWith(const AffPoint& p) const
{
	return (this->distanceTo(p) < AffPoint::sCoincidenceTol);
}

double
AffPoint::distanceSquaredTo(const AffPoint& p) const
{
	double t = (x - p.x);
	double s = t*t;
	t = (y - p.y);
	s += t*t;
	t = (z - p.z);
	s += t*t;

	return s;
}

double
AffPoint::distanceTo(const AffPoint& p) const
{
	return sqrt( distanceSquaredTo(p) );
}
}
