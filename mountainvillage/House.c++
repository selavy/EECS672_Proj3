// House.c++ - concrete subclass of ModelView that represents a House

#include <iostream>
#include <cmath>

#include "House.h"
#include "ShaderIF.h"

#include "CFont.h"
#include "CGLString.h"

typedef float vec2[2];

ShaderIF* House::shaderIF = NULL;
int House::numInstances = 0;
GLuint House::shaderProgram = 0;
GLint House::pvaLoc_wcPosition = -1;
GLint House::pvaLoc_texCoords = -1;
GLint House::ppuLoc_scaleTrans = -1;
GLint House::ppuLoc_housePart = -1;
GLint House::ppuLoc_fontColor = -1;
GLint House::ppuLoc_fontTextureMap = -1;
GLint House::ppuLoc_renderingFontString = -1;

CFont* House::houseLabelFont = NULL;

House::House(double xbIn, double ybIn, double widthIn, double heightIn,
	double roofWidthIn, double roofHeightIn, const std::string& label) :
	xb(xbIn), yb(ybIn), width(widthIn), height(heightIn),
	roofWidth(roofWidthIn), roofHeight(roofHeightIn), houseLabel(NULL)
{
	if (House::shaderProgram == 0)
	{
		// The first time an instance of this class is generated, create the common shader program:
		House::shaderIF = new ShaderIF("House.vsh", "House.fsh");
		House::shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}

	// Now do instance-specific initialization:
	validateData();
	defineModel();
	// finally, create the house label
	buildHouseLabel(label);
	House::numInstances++;
}

House::~House()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
	if (houseLabel != NULL)
		delete houseLabel;
	if (--House::numInstances == 0)
	{
		House::shaderIF->destroy();
		delete House::shaderIF;
		House::shaderIF = NULL;
		House::shaderProgram = 0;
	}
}

void House::buildHouseLabel(const std::string& label)
{
	if (houseLabelFont == NULL)
		return;
	houseLabel = new CGLString(label.c_str(), houseLabelFont, 2);
	houseLabel->setStringDimensions(0.95*width, 0.0);
	double wcLabelWidth = houseLabel->getCurrentRenderWidth();
	double wcLabelHeight = houseLabel->getCurrentRenderHeight();
	double wcXStart = xb - 0.5*wcLabelWidth;
	double wcYStart = yb + height - 1.4*wcLabelHeight;
	houseLabel->setStringOrigin(wcXStart, wcYStart);
}

void House::defineModel()
{
	// We need 11 points: 3 for the roof; 4 for the front wall; 4 for the front door
	vec2 points[11];
	// create the geometry of the roof
	points[0][0] = xb - roofWidth/2.0;
	points[0][1] = yb + height;
	points[1][0] = xb + roofWidth/2.0;
	points[1][1] = points[0][1];
	points[2][0] = xb;
	points[2][1] = yb + height + roofHeight;
	// create the geometry of the front wall
	points[3][0] = xb - width/2.0;
	points[3][1] = yb;
	points[4][0] = xb + width/2.0;
	points[4][1] = yb;
	points[5][0] = points[3][0];
	points[5][1] = yb + height;
	points[6][0] = points[4][0];
	points[6][1] = points[5][1];
	// create the geometry of the front door
	double dw = width / 6.0;
	double dh = height / 4.0;
	points[7][0] = xb - dw/2.0;
	points[7][1] = yb;
	points[8][0] = xb + dw/2.0;
	points[8][1] = yb;
	points[9][0] = points[7][0];
	points[9][1] = yb + dh;
	points[10][0] = points[8][0];
	points[10][1] = points[9][1];

	// send data to GPU:
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 11*sizeof(vec2), points, GL_STATIC_DRAW);
	glVertexAttribPointer(House::pvaLoc_wcPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(House::pvaLoc_wcPosition);
}

void House::fetchGLSLVariableLocations()
{
	if (House::shaderProgram > 0)
	{
		House::pvaLoc_wcPosition = pvAttribLocation(shaderProgram, "wcPosition");
		House::pvaLoc_texCoords = pvAttribLocation(shaderProgram, "texCoords");
		House::ppuLoc_scaleTrans = ppUniformLocation(shaderProgram, "scaleTrans");
		House::ppuLoc_housePart = ppUniformLocation(shaderProgram, "housePart");
		House::ppuLoc_fontTextureMap = ppUniformLocation(shaderProgram, "fontTextureMap");
		House::ppuLoc_fontColor = ppUniformLocation(shaderProgram, "fontColor");
		House::ppuLoc_renderingFontString = ppUniformLocation(shaderProgram, "renderingFontString");
	}
}

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void House::getWCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xb - roofWidth/2.0;
	xyzLimits[1] = xb + roofWidth/2.0;
	xyzLimits[2] = yb;
	xyzLimits[3] = yb + height + roofHeight;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0; // (zmin, zmax) (really 0..0)
}

void House::render()
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderProgram);

	// define the mapping from WC to -1..+1 Logical Device Space:
	float scaleTrans[4];
	computeScaleTrans(scaleTrans);
	glUniform4fv(House::ppuLoc_scaleTrans, 1, scaleTrans);

	glBindVertexArray(vao[0]);
	// draw roof
	glUniform1i(House::ppuLoc_housePart, 0); // '0' means roof
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3); // offset: 0
	// front wall
	glUniform1i(House::ppuLoc_housePart, 1); // '1' means walls of house
	glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);
	// door
	glUniform1i(House::ppuLoc_housePart, 2); // '2' means door
	glDrawArrays(GL_TRIANGLE_STRIP, 7, 4);
	// house label
	renderHouseLabel();
	// restore the previous program
	glUseProgram(pgm);
}

void House::renderHouseLabel() const
{
	if (houseLabel == NULL)
		return;
	float fColor[] = {0.0, 0.0, 0.0, 1.0};
	glUniform4fv(ppuLoc_fontColor, 1, fColor);
	glUniform1i(ppuLoc_renderingFontString, 1);
	houseLabel->render(pvaLoc_wcPosition, pvaLoc_texCoords, ppuLoc_fontTextureMap);
	glUniform1i(ppuLoc_renderingFontString, 0);
}

void House::setHouseLabelFont(std::string font)
{
	houseLabelFont = CFont::getFont(font);
}

void House::validateData()
{
	if (width <= 0.0)
		width = 1.0;
	if (height <= 0.0)
		height = 1.0;
	if (roofWidth < width)
		roofWidth = width;
	if (roofHeight <= 0.0)
		roofHeight = 1.0;
}
