// Mountain.h - concrete subclass of ModelView that represents the Mountain on which
//              our Houses and Trees are placed.

#ifndef MOUNTAIN_H
#define MOUNTAIN_H

class ShaderIF;

#include <GL/gl.h>

#include "ModelView.h"

class Mountain : public ModelView
{
public:
	// A Mountain is modeled as a background cosine curve and a foreground sine curve, each
	// defined over -PI <= theta <= PI. Each curve is defined as:
	//          f(theta) = a*trig(b*theta + c) + d, where 'trig' is cos or sin.
	// The constants 'a' and 'd' are computed from the corresponding y ranges.
	// -PI<=theta<=+PI gets mapped to (xMinIn, xMaxIn) (before "b*theta + c" performed)
	// The sin curve varies from yMin to sin_yMax; the cos curve varies from yMin to cos_yMax
	Mountain(double xMinIn, double xMaxIn, double yMin, double sin_yMax, double cos_yMax,
	         double cos_bIn, double cos_cIn, double sin_bIn, double sin_cIn,
	         int numSamplePointsIn=30);
	virtual ~Mountain();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	// structures to convey geometry to OpenGL/GLSL:
	GLuint vao[1];
	GLuint vbo[2]; // [0] has coordinates; [1] has pv fraction up mountain
	// original defining data
	double xMin, xMax, cos_a, cos_b, cos_c, cos_d, sin_a, sin_b, sin_c, sin_d;
	int numSamplePoints;

	// all instances use same shader program
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_wcPosition;
	static GLint pvaLoc_heightFraction;
	static GLint ppuLoc_scaleTrans;
	static GLint ppuLoc_whichMountain;

	void defineModel();
	static void fetchGLSLVariableLocations();
	void validateData(double yMin, double sin_yMax, double cos_yMax);
};

#endif
