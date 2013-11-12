// MatrixIF.c++

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "MatrixIF.h"

// The first 9 parameters are assumed to be specified
// in World Coordinates (WC).
void wcToECMatrix(double eyeX, double eyeY, double eyeZ,
	double centerX, double centerY, double centerZ,
	double upDx, double upDy, double upDz,
	float m[]) // must be length 16
{
	cryph::Matrix4x4 m4x4 = cryph::Matrix4x4::lookAt(
		cryph::AffPoint(eyeX, eyeY, eyeZ),
		cryph::AffPoint(centerX, centerY, centerZ),
		cryph::AffVector(upDx, upDy, upDz));
	m4x4.extractColMajor(m);
}

// The first 6 parameters are assumed to be specified
// in Eye Coordinates (EC).
void orthogonal(double xmin, double xmax, double ymin,
                double ymax, double zmin, double zmax,
                float m[]) // must be length 16
{
	cryph::Matrix4x4 m4x4 = cryph::Matrix4x4::orthogonal(
			xmin, xmax, ymin, ymax, zmin, zmax);
	m4x4.extractColMajor(m);
}

// The first 7 paramters are assumed to be specified
// in Eye Coordinates (EC).
void oblique( double zpp,
	      double xmin, double xmax, double ymin,
	      double ymax, double zmin, double zmax,
	      double dx, double dy, double dz,
	      float m[] )
{
  cryph::AffVector dvec = cryph::AffVector( dx, dy, dz );
  cryph::Matrix4x4 m4x4 = cryph::Matrix4x4::oblique(
						    zpp,
						    xmin, xmax, ymin, ymax, zmin, zmax,
						    dvec );
  m4x4.extractColMajor(m);
}

// The first 7 parameters are assumed to be specified
// in Eye Coordinates (EC).
void perspective(double zpp,
                 double xmin, double xmax, double ymin,
                 double ymax, double zmin, double zmax,
                float m[]) // must be length 16
{
	cryph::Matrix4x4 m4x4 = cryph::Matrix4x4::perspective(
		zpp,
		xmin, xmax, ymin, ymax, zmin, zmax);
	m4x4.extractColMajor(m);
}
