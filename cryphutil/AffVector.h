// AffVector.h -- 3D vectors associated with a 3D affine space
// This is a subset version of the full cryph::AffVector class
// available as a part of the full cryph source tree available from:
// http://people.eecs.ku.edu/~miller/cryph

#ifndef AFFVECTOR_H
#define AFFVECTOR_H

#include <iostream>
#include <math.h>

#ifndef AFFPOINT_H
namespace cryph { class AffPoint; }
#endif

namespace cryph
{

// indices for extracting components from vectors

const int DW = -1;
const int DX =  0;
const int DY =  1;
const int DZ =  2;

class AffVector
{
public:
	AffVector(); // zero vector
	AffVector(double Dx, double Dy, double Dz=0.0);
	AffVector(const AffVector& v);
	AffVector(const AffPoint& p);
	AffVector(const double xyz[]); // assumes xyz contains 3D components
	AffVector(const float xyz[]);  // assumes xyz contains 3D components
	virtual ~AffVector();

	AffVector	operator=(const AffVector& rhs);
	AffVector	operator+=(const AffVector& rhs);
	AffVector	operator-=(const AffVector& rhs);
	AffVector	operator*=(double f);
	AffVector	operator/=(double f);
	bool	operator==(const AffVector& rhs);
	bool	operator!=(const AffVector& rhs);
	double		operator[](int index) const; // read-only indexing
					// see indexing constants above

	AffVector	operator+(const AffVector& v2) const
				{ return AffVector(dx+v2.dx , dy+v2.dy , dz+v2.dz); }
	AffVector	operator-(const AffVector& v2) const
				{ return AffVector(dx-v2.dx , dy-v2.dy , dz-v2.dz); }
	AffVector	operator-() const { return AffVector(-dx, -dy, -dz); }
	AffVector	operator*(double f) const
				{ return AffVector(f*dx , f*dy , f*dz); }
	AffVector	operator/(double f) const
				{ return AffVector(dx/f , dy/f , dz/f); }

	// ---------- General Methods
	void		arbitraryNormal(AffVector& normal) const;
	AffVector	cross(const AffVector& rhs) const;
	void		decompose(const AffVector& arbitraryVector,
					AffVector& parallel, AffVector& perpendicular) const;
	double		dot(const AffVector& rhs) const
				{ return dx*rhs.dx + dy*rhs.dy + dz*rhs.dz; }
	double		length() const { return sqrt(lengthSquared()); }
	double		lengthSquared() const { return dx*dx + dy*dy + dz*dz; }
	double		maxAbsComponent(int& componentIndex) const;
	double		minAbsComponent(int& componentIndex) const;
	double		normalize();
	double		normalizeToCopy(AffVector& normalizedCopy) const;

	// Among other things, the following two methods are useful for interfacing
	// with OpenGL, especially routines like glBufferData. For example, a caller
	// can allocate an array of an appropriate size, then loop over an array
	// of AffVector instances, invoking the appropriate vComponents method.
	// For example:
	//     float* buf = new float[3*NUM_VECTORS];
	//     for (int i=0 ; i<NUM_VECTORS ; i++)
	//         affVectorArray[i].vComponents(buf, 3*i);
	//     glBufferData(GL_ARRAY_BUFFER, 3*NUM_VECTORS*sizeof(float),
	//                  buf, GL_STATIC_DRAW);
	//     delete [] buf;
	double*		vComponents(double* components, int offset=0) const;
	float*		vComponents(float* components, int offset=0) const;

	// ---------- Class Methods
	// The following method assumes U and W have values (even if all zero).
	// W gets normalized; the component of U perpendicular to W becomes U.
	// Finally V <- W x U.  If W is zero vector, then (xu, yu, zu) are copied
	// into U, V, and W. If the component of U perpendicular to W is zero, then
	// an arbitrary vector perpendicular to W is created and used.)
	static void coordinateSystemFromUW
							(AffVector& U, AffVector& V, AffVector& W);
	// The following is analogous to previous, but with the roles of V and U
	// reversed.
	static void coordinateSystemFromVW
							(AffVector& U, AffVector& V, AffVector& W);

	static AffVector cross(const AffVector& v1, const AffVector& v2);
	static double dot(const AffVector& v1, const AffVector& v2)
					{ return v1.dx*v2.dx + v1.dy*v2.dy + v1.dz*v2.dz; }

	// 1. Special Vectors
	static const AffVector		xu;
	static const AffVector		yu;
	static const AffVector		zu;
	static const AffVector		zeroVector;

	// While the instance variables are public, maximum benefit from the
	// use of thois class is realized when the methods above are used.
	double	dx;
	double	dy;
	double	dz;
};
}

std::ostream&	operator<<(std::ostream& os, const cryph::AffVector& v);
std::istream&	operator>>(std::istream& is, cryph::AffVector& v);
static cryph::AffVector operator*(double f, const cryph::AffVector& v)
	{ return cryph::AffVector(f*v.dx , f*v.dy , f*v.dz); }

#endif
