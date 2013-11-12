// ModelView.c++ - an Abstract Base Class for a combined Model and View for OpenGL

#include <iostream>

#include "ModelView.h"
#include "Controller.h"

ModelView::ModelView()
{
}

ModelView::~ModelView()
{
}

// computeScaleTrans determines the current world coordinate region of
// interest and then uses linearMap to determine how to map coordinates
// in the region of interest to their proper location in Logical Device
// Space. (Returns float[] because glUniform currently favors float[].)
void ModelView::computeScaleTrans(float* scaleTransF) // CLASS METHOD
{
	Controller* c = Controller::getCurrentController();
	double xyzLimits[6];
	c->getWCRegionOfInterest(xyzLimits);

	// preserve aspect ratio. Make "region of interest" wider or taller to
	// match the Controller's viewport aspect ratio.
	double vAR = c->getViewportAspectRatio();
	double xmin = xyzLimits[0], xmax = xyzLimits[1];
	double ymin = xyzLimits[2], ymax = xyzLimits[3];
	if (vAR > 0.0)
	{
		double wHeight = xyzLimits[3] - xyzLimits[2];
		double wWidth = xyzLimits[1] - xyzLimits[0];
		double wAR = wHeight / wWidth;
		if (wAR > vAR)
		{
			// make window wider
			wWidth = wHeight / vAR;
			double xmid = 0.5 * (xmin + xmax);
			xmin = xmid - 0.5*wWidth;
			xmax = xmid + 0.5*wWidth;
		}
		else
		{
			// make window taller
			wHeight = wWidth * vAR;
			double ymid = 0.5 * (ymin + ymax);
			ymin = ymid - 0.5*wHeight;
			ymax = ymid + 0.5*wHeight;
		}
	}

	// We are only concerned with the xy extents for now, hence we will
	// ignore xyzLimits[4] and xyzLimits[5].
	// Map the overall limits to the -1..+1 range expected by the OpenGL engine:
	double scaleTrans[4];
	linearMap(xmin, xmax, -1.0, 1.0, scaleTrans[0], scaleTrans[1]);
	linearMap(ymin, ymax, -1.0, 1.0, scaleTrans[2], scaleTrans[3]);
	for (int i=0 ; i<4 ; i++)
		scaleTransF[i] = static_cast<float>(scaleTrans[i]);
}

// linearMap determines the scale and translate parameters needed in
// order to map a value, f (fromMin <= f <= fromMax) to its corresponding
// value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
void ModelView::linearMap(double fromMin, double fromMax, double toMin, double toMax,
						  double& scale, double& trans) // CLASS METHOD
{
	scale = (toMax - toMin) / (fromMax - fromMin);
	trans = toMin - scale*fromMin;
}

GLint ModelView::ppUniformLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetUniformLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-primitive uniform: '" << name << "'\n";
	return loc;
}

GLint ModelView::pvAttribLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetAttribLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-vertex attribute: '" << name << "'\n";
	return loc;
}
