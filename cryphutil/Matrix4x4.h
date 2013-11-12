// Matrix4x4.h -- 4x4 Matrices
// This is a subset version of the full cryph::Matrix4x4 class
// available as a part of the full cryph source tree available from:
// http://people.eecs.ku.edu/~miller/cryph

#ifndef MATRIX4x4_H
#define MATRIX4x4_H

namespace cryph
{

class AffPoint;
class AffVector;

class Matrix4x4
{
public:

	// --------- basic constructors
	Matrix4x4(); // Identity Matrix
	Matrix4x4(const Matrix4x4& m);
	Matrix4x4(const AffVector& t); // identity matrix with t down fourth column
	Matrix4x4(
		double m11, double m12, double m13, double m14,
		double m21, double m22, double m23, double m24,
		double m31, double m32, double m33, double m34,
		double m41, double m42, double m43, double m44);
	Matrix4x4(const float OGLMatrix[16]);
	Matrix4x4(const double OGLMatrix[16]);

	// --------- destructor
	virtual ~Matrix4x4();

	// --------- operators as methods
	Matrix4x4	operator=(const Matrix4x4& rhs);

	Matrix4x4	operator*=(const Matrix4x4& rhs);
	Matrix4x4	operator*=(double f);
	Matrix4x4	operator+=(const Matrix4x4& rhs);

	AffPoint	operator*(const AffPoint& p) const;
	AffVector	operator*(const AffVector& v) const;

	Matrix4x4	operator*(const Matrix4x4& m2) const;
	Matrix4x4	operator+(const Matrix4x4& m2) const;
	Matrix4x4	operator-(const Matrix4x4& m2) const;

	friend Matrix4x4	operator*(double f, const Matrix4x4& m);
	friend std::ostream&	operator<<(std::ostream& os, const Matrix4x4& m);
	friend std::istream&	operator>>(std::istream& is, Matrix4x4& m);

	// ---------- General Methods

				// Two extraction routines useful in OpenGL, etc.
				// returns in column-major order
	float*		extractColMajor(float m[16]) const;  // GLfloat
	double*		extractColMajor(double m[16]) const; // GLdouble

				// in the following, 'a' and 'b' are asssumed to be arrays[4]
	void		multiply(const double a[], double b[]) const;
	void		multiply(const float a[], float b[]) const;

	// common viewing interfaces
	static Matrix4x4 lookAt(const AffPoint& eye,
							const AffPoint& center,
							const AffVector& up);

	static Matrix4x4 orthogonal(double xmin, double xmax, double ymin,
	                            double ymax, double zmin, double zmax);
	static Matrix4x4 oblique(double zpp, double xmin, double xmax,
	                         double ymin, double ymax, double zmin, double zmax,
	                         const AffVector& projDir);
	static Matrix4x4 perspective(double zpp, double xmin, double xmax,
	                         double ymin, double ymax, double zmin, double zmax);

	// common modeling matrix interfaces

	static Matrix4x4 xRotationDegrees(double angle);
	static Matrix4x4 yRotationDegrees(double angle);
	static Matrix4x4 zRotationDegrees(double angle);

    // ---------- Global constants

	// 1. Special Matrices
	static const Matrix4x4		IdentityMatrix;
	static const Matrix4x4		ZeroMatrix;

protected:

	// --------- instance variables
	double	mElem[4][4];

private:

	// --------- private methods
			void	copy(const Matrix4x4& rhs);
};

}

#endif
