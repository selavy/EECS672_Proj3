// Tree.h - concrete subclass of ModelView that represents a Tree

#ifndef TREE_H
#define TREE_H

class ShaderIF;

#include <GL/gl.h>

#include "ModelView.h"

class Tree : public ModelView
{
public:
	// A tree is defined with center of base at (xb,yb). The leafy tree top
	// is a sphere of radius rTreeTop centered 'height' units directy up from
	// the base. The trunk is drawn as a circular arc of radius rTrunk. The
	// center of the arc is (xb +/- rTrunk, yb + height).
	// REQUIREMENT: rTrunk >= d.
	Tree(double xbIn, double ybIn, double rTreeTopIn, double rTrunkIn, double heightIn,
		int numTreeTopPointsIn=30, int numTrunkPointsIn=25);
	virtual ~Tree();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	// structures to convey geometry to OpenGL/GLSL:
	GLuint vao[1];
	GLuint vbo[1]; // Stores both trunk and treetop points
	int numTrunkTriStripPoints;
	// original defining data
	double xb, yb, rTreeTop, rTrunk, height;
	int numTreeTopPoints, numTrunkPoints;

	// all instances use same shader program
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_wcPosition;
	static GLint ppuLoc_scaleTrans;
	static GLint ppuLoc_treePart;

	void defineModel();
	static void fetchGLSLVariableLocations();
	void validateData();
};

#endif
