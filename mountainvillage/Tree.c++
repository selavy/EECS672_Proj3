// Tree.c++ - concrete subclass of ModelView that represents a Tree

#include <iostream>
#include <cmath>

#include "Tree.h"
#include "ShaderIF.h"

typedef float vec2[2];

ShaderIF* Tree::shaderIF = NULL;
int Tree::numInstances = 0;
GLuint Tree::shaderProgram = 0;
GLint Tree::pvaLoc_wcPosition = -1;
GLint Tree::ppuLoc_scaleTrans = -1;
GLint Tree::ppuLoc_treePart = -1;

Tree::Tree(double xbIn, double ybIn, double rTreeTopIn, double rTrunkIn, double heightIn,
	int numTreeTopPointsIn, int numTrunkPointsIn) :
	xb(xbIn), yb(ybIn), rTreeTop(rTreeTopIn), rTrunk(rTrunkIn), height(heightIn),
	numTreeTopPoints(numTreeTopPointsIn), numTrunkPoints(numTrunkPointsIn)
{
	if (Tree::shaderProgram == 0)
	{
		// The first time an instance of this class is generated, create the common shader program:
		Tree::shaderIF = new ShaderIF("Tree.vsh", "Tree.fsh");
		Tree::shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}

	// Now do instance-specific initialization:
	validateData();
	defineModel();
	Tree::numInstances++;
}

Tree::~Tree()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
	if (--Tree::numInstances == 0)
	{
		Tree::shaderIF->destroy();
		delete Tree::shaderIF;
		Tree::shaderIF = NULL;
		Tree::shaderProgram = 0;
	}
}

void Tree::defineModel()
{
	numTrunkTriStripPoints = 2 * numTrunkPoints;
	vec2* points = new vec2[numTrunkTriStripPoints + numTreeTopPoints];
	// create the geometry of the trunk
	double leftTrunkCenterX = xb - rTrunk;
	double leftTrunkCenterY = yb + height;
	double trunkTheta = asin(height/rTrunk);
	double dTheta = trunkTheta / (numTrunkPoints - 1);
	double theta = -trunkTheta;
	for (int i=0 ; i<numTrunkTriStripPoints ; i+=2)
	{
		points[i][0] = leftTrunkCenterX + rTrunk * cos(theta);
		points[i][1] = leftTrunkCenterY + rTrunk * sin(theta);
		// right-hand trunk part is mirror image about center of tree:
		points[i+1][0] = 2.0*xb - points[i][0];
		points[i+1][1] = points[i][1];
		theta += dTheta;
	}
	// create the geometry of the tree top; append at end of "points" array
	theta = 0.0;
	// first point is center of circle
	points[numTrunkTriStripPoints][0] = xb;
	points[numTrunkTriStripPoints][1] = yb + height;
	// Then (numTreeTopPoints-1) points around perimeter, so:
	dTheta = 2.0*M_PI / (numTreeTopPoints - 2);
	for (int i=1 ; i<numTreeTopPoints ; i++)
	{
		points[numTrunkTriStripPoints+i][0] = xb + rTreeTop * cos(theta);
		points[numTrunkTriStripPoints+i][1] = yb + height + rTreeTop * sin(theta);
		theta += dTheta;
	}

	// send vertex data to GPU:
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, (numTrunkTriStripPoints + numTreeTopPoints)*sizeof(vec2),
		points, GL_STATIC_DRAW);
	glVertexAttribPointer(Tree::pvaLoc_wcPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Tree::pvaLoc_wcPosition);

	delete [] points;
}

void Tree::fetchGLSLVariableLocations()
{
	if (Tree::shaderProgram > 0)
	{
		Tree::pvaLoc_wcPosition = pvAttribLocation(shaderProgram, "wcPosition");
		Tree::ppuLoc_scaleTrans = ppUniformLocation(shaderProgram, "scaleTrans");
		Tree::ppuLoc_treePart = ppUniformLocation(shaderProgram, "treePart");
	}
}

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Tree::getWCBoundingBox(double* xyzLimits) const
{
	double dx1 = xb - rTreeTop; // min x of tree top
	double trunkTheta = asin(height/rTrunk);
	double dx2 = xb - rTrunk*(1.0 - cos(trunkTheta)); // min x of base
	if (dx1 < dx2)
		xyzLimits[0] = dx1;
	else
		xyzLimits[0] = dx2;
	xyzLimits[1] = 2.0*xb - xyzLimits[0];
	xyzLimits[2] = yb;
	xyzLimits[3] = yb + height + rTreeTop;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0; // (zmin, zmax) (really 0..0)
}

void Tree::render()
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderProgram);

	// define the mapping from WC to -1..+1 Logical Device Space:
	float scaleTrans[4];
	computeScaleTrans(scaleTrans);
	glUniform4fv(Tree::ppuLoc_scaleTrans, 1, scaleTrans);

	glBindVertexArray(vao[0]);
	// draw trunk first because tree top overwrites it
	glUniform1i(Tree::ppuLoc_treePart, 0); // '0' means trunk
	int numTrunkTriStripPoints = 2 * numTrunkPoints;
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTrunkTriStripPoints); // offset: 0
	// then draw tree top
	glUniform1i(Tree::ppuLoc_treePart, 1); // '1' means tree top
	//                            offset:                 num points:
	glDrawArrays(GL_TRIANGLE_FAN, numTrunkTriStripPoints, numTreeTopPoints);
	// restore the previous program
	glUseProgram(pgm);
}

void Tree::validateData()
{
	if (rTreeTop <= 0.0)
		rTreeTop = 1.0;
	if (height <= 0.0)
		height = 1.0;
	if (rTrunk < height)
		rTrunk = height;
	if (numTreeTopPoints < 5)
		numTreeTopPoints = 5;
	if (numTrunkPoints < 2)
		numTrunkPoints = 2;
}
