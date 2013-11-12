// ModelView.h - an Abstract Base Class for a combined Model and View for OpenGL
//
// This definition defines class ModelView as an Abstract Base Class by factoring out
// a minimal set of methods that can be used by a Controller to manage a collection of
// abstract models. The public methods here are those required by the Controller; the
// protected methods implement common operations likely required by all concrete subclasses,
// hence they can be implemented once in this abstract base class. Specifically:
// 1. Methods "getWCBoundingBox" and "render" are now pure virtual.
// 2. Method "handleCommand" is given a default (empty) implementation. Subclasses need only
//    override if they actually want to handle commands from the keyboard or elsewhere.
// 3. The protected class method "computeScaleTrans" is designed to be used by
//    ModelView subclasses to fetch the scale and translation terms in x and y that
//    implement a basic window-viewport transformation. The implementation first queries
//    the Controller for its overall bounding box. That box is used as the world coordinate
//    window, and the scale/translation terms map points inside that window into the
//    -1 <= [x,y] <= +1 logical viewport range expected by OpenGL. Subclasses will generally
//    simply pass these scale and translation terms to their vertex shaders.
// 4. The GLSL shader variable lookup routines are also available here via protected methods.

#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <string>
#include <GL/gl.h>

class ModelView
{
public:
	ModelView();
	virtual ~ModelView();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	virtual void getWCBoundingBox(double* xyzLimits) const = 0;
	virtual void handleCommand(unsigned char key, double ldsX, double ldsY) { }
	virtual void handleSpecialCommand(int key, double ldsX, double ldsY) { }
	virtual void render() = 0;

protected:
	static void computeScaleTrans(float* scaleTransF);
	static void linearMap(double fromMin, double fromMax,
		double toMin, double toMax, double& scale, double& trans);
	// "pp": "per-primitive"; "pv": "per-vertex"
	static GLint ppUniformLocation(GLuint glslProgram, const std::string& name);
	static GLint pvAttribLocation(GLuint glslProgram, const std::string& name);
};

#endif
