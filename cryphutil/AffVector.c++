// AffVector.c++ -- 3D vectors associated with a 3D affine space
// This is a subset version of the full cryph::AffVector class
// available as a part of the full cryph source tree available from:
// http://people.eecs.ku.edu/~miller/cryph

#include <stdlib.h>

#include "AffPoint.h"
#include "AffVector.h"

using namespace std;

static double BasicDistanceTol = 1.0e-4;
static double BasicUnitTol = 1.0e-6;

namespace cryph
{
// 1. Special Vectors
const AffVector	AffVector::xu = AffVector( 1.0 , 0.0 , 0.0 );
const AffVector	AffVector::yu = AffVector( 0.0 , 1.0 , 0.0 );
const AffVector	AffVector::zu = AffVector( 0.0 , 0.0 , 1.0 );
const AffVector	AffVector::zeroVector = AffVector( 0.0 , 0.0 , 0.0 );

// ---------- General Class Methods

void
AffVector::coordinateSystemFromUW(AffVector& U, AffVector& V, AffVector& W)
{
	// We assume that U and W have values (even if all zero). We use W (after
	// normalization) as the W axis. The component of U perpendicular to W
	// (and after normalization) becomes U. Finally V is the cross product
	// of W and U. If troubles arise, suitable defaults are generated so that
	// this routine always does something reasonable.

	if (W.normalize() < BasicDistanceTol)
	{
		U = AffVector::xu; V = AffVector::yu; W = AffVector::zu;
	}
	else
	{
		AffVector par, perp;

		W.decompose(U,par,perp);
		if (perp.normalizeToCopy(U) < BasicDistanceTol)
		{
			W.arbitraryNormal(U);
			(void) U.normalize();
		}
		V = AffVector::cross(W,U);
		V.normalize(); // for numerical reasons
	}
}

void
AffVector::coordinateSystemFromVW(AffVector& U, AffVector& V, AffVector& W)
{
	// We assume that V and W have values (even if all zero). We use W (after
	// normalization) as the W axis. The component of V perpendicular to W
	// (and after normalization) becomes V. Finally U is the cross product
	// of V and W. If troubles arise, suitbale defaults are generated so that
	// this routine always does something reasonable.

	if (W.normalize() < BasicDistanceTol)
	{
		U = AffVector::xu; V = AffVector::yu; W = AffVector::zu;
	}
	else
	{
		AffVector par, perp;

		W.decompose(V,par,perp);
		if (perp.normalizeToCopy(V) < BasicDistanceTol)
		{
			W.arbitraryNormal(V);
			(void) V.normalize();
		}
		U = AffVector::cross(V,W);
		U.normalize(); // for numerical reasons
	}
}

AffVector
AffVector::cross(const AffVector& v1, const AffVector& v2)
{
	AffVector	result;

	result.dx = v1.dy*v2.dz - v1.dz*v2.dy;

	result.dy = v1.dz*v2.dx - v1.dx*v2.dz;

	result.dz = v1.dx*v2.dy - v1.dy*v2.dx;

	return result;
}

// --------- basic constructors

AffVector::AffVector() : dx(0), dy(0), dz(0)
{
}

AffVector::AffVector(double Dx, double Dy, double Dz) : dx(Dx), dy(Dy), dz(Dz)
{
}
AffVector::AffVector(const AffVector& v) : dx(v.dx), dy(v.dy), dz(v.dz)
{
}

// --------- other constructors

AffVector::AffVector(const AffPoint& p) : dx(p[X]), dy(p[Y]), dz(p[Z])
{
}
AffVector::AffVector(const double xyz[3]) : dx(xyz[0]), dy(xyz[1]), dz(xyz[2])
{
}
AffVector::AffVector(const float xyz[3]) : dx(xyz[0]), dy(xyz[1]), dz(xyz[2])
{
}

// --------- destructor

AffVector::~AffVector()
{
}

// --------- overloaded operators

AffVector
AffVector::operator=(const AffVector& rhs)
{
	dx = rhs.dx; dy = rhs.dy; dz = rhs.dz;
	return *this;
}

AffVector
AffVector::operator+=(const AffVector& rhs)
{
	dx += rhs.dx; dy += rhs.dy; dz += rhs.dz;
	return *this;
}

AffVector
AffVector::operator-=(const AffVector& rhs)
{
	dx -= rhs.dx; dy -= rhs.dy; dz -= rhs.dz;
	return *this;
}

AffVector
AffVector::operator*=(double f)
{
	dx *= f;
	dy *= f;
	dz *= f;

	return *this;
}

AffVector
AffVector::operator/=(double f)
{
	dx /= f;
	dy /= f;
	dz /= f;

	return *this;
}

bool
AffVector::operator==(const AffVector& rhs)
{
	AffVector	u, v;

	double	thisL = this->normalizeToCopy(u);
	double	rhsL  = rhs.normalizeToCopy(v);

	if (fabs(thisL-rhsL) < BasicDistanceTol)
		if (thisL > BasicDistanceTol)
		{
			if (fabs(AffVector::dot(u,v)-1.0) < BasicUnitTol)
				return true;
		}
		else // both are zero vectors, hence:
			return true;

	return false;
}

bool
AffVector::operator!=(const AffVector& rhs)
{
	return ! this->operator==(rhs);
}

double
AffVector::operator[](int index) const
{
	// read-only indexing

	switch (index)
	{
		case DW:
			return 0.0; // w == 0
		case DX:
			return dx;
		case DY:
			return dy;
		case DZ:
			return dz;
	}

	return 0.0;
}

// operators as external functions, not methods

ostream&
operator<<(ostream& os, const AffVector& v)
{
	os << '(' << v[DX] << ", " << v[DY] << ", " << v[DZ] << ')';
	return os;
}
istream&
operator>>(istream& is, AffVector& v)
{
	double x, y, z;
	is >> x >> y >> z;
	v = AffVector(x,y,z);

	return is;
}

// ---------- General Instance Methods

void
AffVector::arbitraryNormal(AffVector& normal) const
{
	// Create an arbitrary vector perpendicular to "this"

	const int ignoreX = 0;
	const int ignoreY = 1;
	const int ignoreZ = 2;

	double	Dx = fabs(this->dx);
	double	Dy = fabs(this->dy);
	double	Dz = fabs(this->dz);

	int		ignore = ignoreX;
	if (Dx >= Dy)
		if (Dy > Dz)
			ignore = ignoreZ;
		else
			ignore = ignoreY;
	else if (Dx > Dz)
		ignore = ignoreZ;

	// set the ignored component to zero, invert the other two, and negate the
	// one with the largest absolute value.

	switch (ignore)
	{
		case ignoreX:
			normal.dx = 0.0;
			if (Dy > Dz)
			{
				normal.dy = this->dz; normal.dz = -this->dy;
			}
			else
			{
				normal.dy = -this->dz; normal.dz = this->dy;
			}
			break;
		case ignoreY:
			normal.dy = 0.0;

			// to get "preferred result" when v1 parallel to z-axis, we add the
			// "or dx < BasicDistanceTol" business here

			if ( (Dx > Dz) || (Dx < BasicDistanceTol) )
			{
				normal.dx = this->dz; normal.dz = -this->dx;
			}
			else
			{
				normal.dx = -this->dz; normal.dz = this->dx;
			}
			break;
		case ignoreZ:
			normal.dz = 0.0;
			if (Dx > Dy)
			{
				normal.dx = this->dy; normal.dy = -this->dx;
			}
			else
			{
				normal.dx = -this->dy; normal.dy = this->dx;
			}
	}
}

double* AffVector::vComponents(double* components, int offset) const
{
	components[offset  ] = dx;
	components[offset+1] = dy;
	components[offset+2] = dz;
	return components;
}
	
float* AffVector::vComponents(float* components, int offset) const
{
	components[offset  ] = static_cast<float>(dx);
	components[offset+1] = static_cast<float>(dy);
	components[offset+2] = static_cast<float>(dz);
	return components;
}
	
AffVector
AffVector::cross(const AffVector& rhs) const
{
	AffVector	result;

	result.dx = dy*rhs.dz - dz*rhs.dy;

	result.dy = dz*rhs.dx - dx*rhs.dz;

	result.dz = dx*rhs.dy - dy*rhs.dx;

	return result;
}

void
AffVector::decompose(const AffVector& arbitraryVector,
			AffVector& parallel, AffVector& perpendicular) const
{
	AffVector	u;
	this->normalizeToCopy(u);

	parallel = AffVector::dot(arbitraryVector,u) * u;
	perpendicular = arbitraryVector - parallel;
}

double
AffVector::maxAbsComponent(int& componentIndex) const
{
	componentIndex = DX;
	double v = fabs(dx);
	if (fabs(dy) > v)
	{
		v = fabs(dy); componentIndex = DY;
	}
	if (fabs(dz) > v)
	{
		v = fabs(dz); componentIndex = DZ;
	}
	return v;
}

double
AffVector::minAbsComponent(int& componentIndex) const
{
	componentIndex = DX;
	double v = fabs(dx);
	if (fabs(dy) < v)
	{
		v = fabs(dy); componentIndex = DY;
	}
	if (fabs(dz) < v)
	{
		v = fabs(dz); componentIndex = DZ;
	}
	return v;
}

double
AffVector::normalize()
{
	double l = length();
	if (l > 0.0)
	{
		dx /= l; dy /= l; dz /= l;
	}
	return l;
}

double
AffVector::normalizeToCopy(AffVector& normalizedCopy) const
{
	normalizedCopy = *this;
	return normalizedCopy.normalize();
}
}
