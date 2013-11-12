// MatrixIF.h

#ifndef MATRIXIF_H
#define MATRIXIF_H

// The first 9 parameters are assumed to be specified
// in World Coordinates (WC).
void wcToECMatrix(double eyeX, double eyeY, double eyeZ,
	double centerX, double centerY, double centerZ,
	double upDx, double upDy, double upDz,
	float m[]); // must be length 16

// The first 6 parameters are assumed to be specified
// in Eye Coordinates (EC).
void orthogonal(double xmin, double xmax, double ymin,
                double ymax, double zmin, double zmax,
                float m[]); // must be length 16

void oblique( double zpp,
	      double xmin, double xmax, double ymin,
	      double ymax, double zmin, double zmax,
	      double dx, double dy, double dz,
	      float m[]); // must be length 16

// The first 7 parameters are assumed to be specified
// in Eye Coordinates (EC).
void perspective(double zpp,
                 double xmin, double xmax, double ymin,
                 double ymax, double zmin, double zmax,
                 float m[]); // must be length 16

#endif
