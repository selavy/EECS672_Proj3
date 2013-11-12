// Matrix4x4.c++ -- 4x4 transformation matrices
// This is a subset version of the full cryph::Matrix4x4 class
// available as a part of the full cryph source tree available from:
// http://people.eecs.ku.edu/~miller/cryph

#include <stdlib.h>
#include <math.h>

#include <iomanip>
using namespace std;

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

static double BasicDistanceTol = 1.0e-4;
static double BasicUnitTol = 1.0e-6;
static const double degreesToRadians = M_PI/180.0;

namespace cryph
{

// ---------- public Global constants

// 1. Special Matrices
const Matrix4x4 Matrix4x4::IdentityMatrix =
	Matrix4x4(
		1.0 , 0.0 , 0.0 , 0.0 ,
		0.0 , 1.0 , 0.0 , 0.0 ,
		0.0 , 0.0 , 1.0 , 0.0 ,
		0.0 , 0.0 , 0.0 , 1.0
		);

const Matrix4x4 Matrix4x4::ZeroMatrix =
	Matrix4x4(
		0.0 , 0.0 , 0.0 , 0.0 ,
		0.0 , 0.0 , 0.0 , 0.0 ,
		0.0 , 0.0 , 0.0 , 0.0 ,
		0.0 , 0.0 , 0.0 , 0.0
		);

// END: public Global constants

// --------- basic constructors

Matrix4x4::Matrix4x4()
{
	this->copy(Matrix4x4::IdentityMatrix);
}

Matrix4x4::Matrix4x4(const Matrix4x4& m)
{
	this->copy(m);
}

Matrix4x4::Matrix4x4(const AffVector& t)
{
	this->copy(Matrix4x4::IdentityMatrix);
	mElem[0][3] = t[DX];
	mElem[1][3] = t[DY];
	mElem[2][3] = t[DZ];
}

Matrix4x4::Matrix4x4(
	double m11, double m12, double m13, double m14,
	double m21, double m22, double m23, double m24,
	double m31, double m32, double m33, double m34,
	double m41, double m42, double m43, double m44)
{
	mElem[0][0] = m11; mElem[0][1] = m12; mElem[0][2] = m13; mElem[0][3] = m14;
	mElem[1][0] = m21; mElem[1][1] = m22; mElem[1][2] = m23; mElem[1][3] = m24;
	mElem[2][0] = m31; mElem[2][1] = m32; mElem[2][2] = m33; mElem[2][3] = m34;
	mElem[3][0] = m41; mElem[3][1] = m42; mElem[3][2] = m43; mElem[3][3] = m44;
}

Matrix4x4::Matrix4x4(const float OGLMatrix[16])
{
	int		k = 0;
	for (int col=0 ; col<4 ; col++)
		for (int row=0 ; row<4 ; row++)
			this->mElem[row][col] = OGLMatrix[k++];
}

Matrix4x4::Matrix4x4(const double OGLMatrix[16])
{
	int		k = 0;
	for (int col=0 ; col<4 ; col++)
		for (int row=0 ; row<4 ; row++)
			this->mElem[row][col] = OGLMatrix[k++];
}


// --------- destructor

Matrix4x4::~Matrix4x4()
{
}

// --------- operators as methods

Matrix4x4
Matrix4x4::operator=(const Matrix4x4& rhs)
{
	this->copy(rhs);
	return *this;
}

Matrix4x4
Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	*this = (*this) * rhs;
	return *this;
}

Matrix4x4
Matrix4x4::operator*=(double f)
{
	for (int i=0 ; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			this->mElem[i][j] *= f;
	return *this;
}

Matrix4x4
Matrix4x4::operator+=(const Matrix4x4& rhs)
{
	for (int i=0 ; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			this->mElem[i][j] += rhs.mElem[i][j];
	return *this;
}

AffPoint
Matrix4x4::operator*(const AffPoint& p) const
{
	double	c[4] = { p[X] , p[Y] , p[Z], 1.0 };
	double	d[4];

	for (int i=0 ; i<4 ; i++)
	{
		double sum = 0.0;
		for (int j=0 ; j<4 ; j++)
			sum += (this->mElem[i][j] * c[j]);
		d[i] = sum;
	}

	if (fabs(d[3]) > BasicDistanceTol)
		return AffPoint(d[0]/d[3],d[1]/d[3],d[2]/d[3]);

	return AffPoint(d[0],d[1],d[2]);
}

AffVector
Matrix4x4::operator*(const AffVector& v) const
{
	double	c[3] = { v[DX] , v[DY] , v[DZ] };
	double	d[3];

	for (int i=0 ; i<3 ; i++)
	{
		double sum = 0.0;
		for (int j=0 ; j<3 ; j++)
			sum += (this->mElem[i][j] * c[j]);
		d[i] = sum;
	}
	return AffVector(d[0],d[1],d[2]);
}

// --------- operators

Matrix4x4
Matrix4x4::operator*(const Matrix4x4& m2) const
{
	Matrix4x4 mOut;

	for (int i=0 ; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
		{
			double sum = 0.0;
			for (int k=0 ; k<4 ; k++)
				sum += mElem[i][k]*m2.mElem[k][j];
			mOut.mElem[i][j] = sum;
		}

	return mOut;
}

Matrix4x4
Matrix4x4::operator+(const Matrix4x4& m2) const
{
	Matrix4x4	result(*this);

	for (int i=0 ; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			result.mElem[i][j] += m2.mElem[i][j];
	return result;
}

Matrix4x4
Matrix4x4::operator-(const Matrix4x4& m2) const
{
	Matrix4x4	result(*this);

	for (int i=0 ; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			result.mElem[i][j] -= m2.mElem[i][j];
	return result;
}

Matrix4x4
operator*(double f, const Matrix4x4& m) // external friend function
{
	Matrix4x4	result(m);

	for (int i=0 ; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			result.mElem[i][j] *= f;
	return result;
}

ostream&
operator<<(ostream& os, const Matrix4x4& m)
{
	os << '(';

	for (int i=0 ; i<4 ; i++)
	{
		os << '(';
		for (int j=0 ; j<3 ; j++)
			os << m.mElem[i][j] << ", ";
		os << m.mElem[i][3] << ')';
		// if this is not the last row
		if (i < (3))
			os << ", ";
	}
	os << ')';
	return os;
}

istream&
operator>>(istream& is, Matrix4x4& m)
{
	for (int i=0 ; i<4 ; i++)
	{
		for (int j=0 ; j<4 ; j++)
			is >> m.mElem[i][j];
	}
	return is;
}

// --------- General Methods

void
Matrix4x4::copy(const Matrix4x4& rhs)
{
	for (int i=0 ; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			this->mElem[i][j] = rhs.mElem[i][j];
}

float*
Matrix4x4::extractColMajor(float m[16]) const
{
	int		k = 0;
	for (int col=0 ; col<4 ; col++)
		for (int row=0 ; row<4 ; row++)
			m[k++] = static_cast<float>(this->mElem[row][col]);
	return &m[0];
}

double*
Matrix4x4::extractColMajor(double m[16]) const
{
	int		k = 0;
	for (int col=0 ; col<4 ; col++)
		for (int row=0 ; row<4 ; row++)
			m[k++] = this->mElem[row][col];
	return &m[0];
}

Matrix4x4 Matrix4x4::lookAt(const cryph::AffPoint& eye,
		const cryph::AffPoint& center, const cryph::AffVector& up)
{
	AffVector w = eye - center;
	if (w.normalize() < BasicDistanceTol)
	{
		std::cerr << "Matrix4x4::lookAt: eye and center are coincident.\n";
		return IdentityMatrix;
	}
	AffVector par, v;
	w.decompose(up,par, v);
	if (v.normalize() < BasicUnitTol)
	{
		std::cerr << "Matrix4x4::lookAt: 'up' is zero or parallel to line of sight.\n";
		return IdentityMatrix;
	}
	AffVector u = v.cross(w);
	u.normalize();
	double tx = -u.dot(eye);
	double ty = -v.dot(eye);
	double tz = -w.dot(eye);
	return Matrix4x4(
	               u[DX],u[DY],u[DZ], tx,
				   v[DX],v[DY],v[DZ], ty,
				   w[DX],w[DY],w[DZ], tz,
	                 0.0,  0.0,  0.0, 1.0);
}

Matrix4x4 Matrix4x4::orthogonal(double xmin, double xmax, double ymin,
	                            double ymax, double zmin, double zmax)
{
	if ((xmin >= xmax) || (ymin >= ymax) || (zmin >= zmax))
	{
		std::cerr << "Matrix4x4::orthogonal: one or more min values >= max.\n";
		std::cerr << "\tGiven: (xmin,xmax,ymin,ymax,zmin,zmax) = (" << xmin << ", " << xmax << ", "
		          << ymin << ", " << ymax << ", " << zmin << ", " << zmax << '\n';
		return IdentityMatrix;
	}
	return Matrix4x4(
		2.0/(xmax-xmin), 0.0, 0.0, -(xmax+xmin)/(xmax-xmin),
		0.0, 2.0/(ymax-ymin), 0.0, -(ymax+ymin)/(ymax-ymin),
		0.0, 0.0, -2.0/(zmax-zmin), (zmax+zmin)/(zmax-zmin),
		0.0, 0.0, 0.0, 1.0
	);
}

Matrix4x4 Matrix4x4::oblique(double zpp, double xmin, double xmax,
	                         double ymin, double ymax, double zmin, double zmax,
                             const cryph::AffVector& projDir)
{
	if ((xmin >= xmax) || (ymin >= ymax) || (zmin >= zmax))
	{
		std::cerr << "Matrix4x4::oblique: one or more min values >= max.\n";
		std::cerr << "\tGiven: (xmin,xmax,ymin,ymax,zmin,zmax) = (" << xmin << ", " << xmax << ", "
		          << ymin << ", " << ymax << ", " << zmin << ", " << zmax << '\n';
		return IdentityMatrix;
	}
	double pDZ = projDir[DZ];
	if (fabs(pDZ) < BasicUnitTol)
	{
		std::cerr << "Matrix4x4::oblique: invalid projection direction: " << projDir << '\n';
		return IdentityMatrix;
	}
	double dx = xmax - xmin;
	double dy = ymax - ymin;
	double dz = zmin - zmax; // note swapped order
	double pDX = projDir[DX];
	double pDY = projDir[DY];
	return cryph::Matrix4x4(
		2.0/dx, 0.0, -2.0*pDX/(pDZ*dx), 2.0*zpp*pDX/(pDZ*dx) - (xmin+xmax)/dx,
		0.0, 2.0/dy, -2.0*pDY/(pDZ*dy), 2.0*zpp*pDY/(pDZ*dy) - (ymin+ymax)/dy,
		0.0, 0.0, 2.0/dz, -(zmin + zmax)/dz,
		0.0, 0.0, 0.0, 1.0
	);
}

Matrix4x4 Matrix4x4::perspective(double zpp, double xmin, double xmax,
	                         double ymin, double ymax, double zmin, double zmax)
{
	if ((xmin >= xmax) || (ymin >= ymax) || (zmin >= zmax))
	{
		std::cerr << "Matrix4x4::perspective: one or more min values >= max.\n";
		std::cerr << "\tGiven: (xmin,xmax,ymin,ymax,zmin,zmax) = (" << xmin << ", " << xmax << ", "
		          << ymin << ", " << ymax << ", " << zmin << ", " << zmax << '\n';
		return IdentityMatrix;
	}
	if ((zmax >= 0.0) || (zpp >= 0.0))
	{
		std::cerr << "Matrix4x4::perspective: must have zmin < zmax < 0 AND zpp < 0\n";
		std::cerr << "\tGiven: zpp = " << zpp << ", zmin = " << zmin << ", zmax = " << zmax << '\n';
		return IdentityMatrix;
	}
	return Matrix4x4(
		-2.0*zpp/(xmax - xmin),                    0.0, (xmax + xmin)/(xmax - xmin),                            0.0,
		                   0.0, -2.0*zpp/(ymax - ymin), (ymax + ymin)/(ymax - ymin),                            0.0,
		                   0.0,                    0.0, (zmax + zmin)/(zmax - zmin), -2.0*zmin*zmax / (zmax - zmin),
	                       0.0,                    0.0,                        -1.0,                           0.0);
}

void
Matrix4x4::multiply(const double a[], double b[]) const
{
	for (int i=0 ; i<4 ; i++)
	{
		double sum = 0.0;
		for (int j=0 ; j<4 ; j++)
			sum += mElem[i][j]*a[j];
		b[i] = sum;
	}
}

void
Matrix4x4::multiply(const float a[], float b[]) const
{
	for (int i=0 ; i<4 ; i++)
	{
		float sum = 0.0;
		for (int j=0 ; j<4 ; j++)
			sum += static_cast<float>(mElem[i][j])*a[j];
		b[i] = sum;
	}
}

Matrix4x4 Matrix4x4::xRotationDegrees(double angle)
{
	double radians = angle * degreesToRadians;
	double c = cos(radians), s = sin(radians);
	return Matrix4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0,   c,  -s, 0.0,
		0.0,   s,   c, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

Matrix4x4 Matrix4x4::yRotationDegrees(double angle)
{
	double radians = angle * degreesToRadians;
	double c = cos(radians), s = sin(radians);
	return Matrix4x4(
		  c, 0.0,   s, 0.0,
		0.0, 1.0, 0.0, 0.0,
		 -s, 0.0,   c, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

Matrix4x4 Matrix4x4::zRotationDegrees(double angle)
{
	double radians = angle * degreesToRadians;
	double c = cos(radians), s = sin(radians);
	return Matrix4x4(
		  c,  -s, 0.0, 0.0,
		  s,   c, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

} // end namespace
