// mountainvillage.c++: Driver for creating Houses and Trees on Mountains

#include <iostream>
#include <fstream>
#include <string>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "Controller.h"
#include "House.h"
#include "Mountain.h"
#include "Tree.h"

void makeDefaultScene(Controller& c)
{
	c.addModel( new Mountain(-200, 200, -20, 20, 60, 1, 0, 1, 0) );
	c.addModel( new Tree(-150.0, -120.0, 18.0, 60.0, 45.0) );
	c.addModel( new House(-15.0, -150.0, 85.0, 70.0, 95.0, 35.0, "Rock Chalk") );
	c.addModel( new House(60.0, -10.0, 15.0, 12.0, 25.0, 10.0, "Jayhawk") );
	c.addModel( new Tree(120.0, -220.0, 43.0, 220.0, 125.0) );
}

House* readHouse(std::istream& is)
{
	double xb, yb, width, height, roofWidth, roofHeight;
	is >> xb >> yb >> width >> height >> roofWidth >> roofHeight;
	std::string label;
	getline(is, label);
	return new House(xb, yb, width, height, roofWidth, roofHeight, label);
}

Mountain* readMountain(std::istream& is)
{
	double xMin, xMax, yMin, sin_yMax, cos_yMax, cos_b, cos_c, sin_b, sin_c;
	int numSamplePts;
	is >> xMin >> xMax >> yMin >> sin_yMax >> cos_yMax >> cos_b >> cos_c >> sin_b >> sin_c;
	is >> numSamplePts;
	return new Mountain(xMin, xMax, yMin, sin_yMax, cos_yMax, cos_b, cos_c, sin_b, sin_c,
		numSamplePts);
}

Tree* readTree(std::istream& is)
{
	double xb, yb, rTreeTop, rTrunk, height;
	int nTreeTopPts, nTrunkPts;
	is >> xb >> yb >> rTreeTop >> rTrunk >> height >> nTreeTopPts >> nTrunkPts;
	return new Tree(xb, yb, rTreeTop, rTrunk, height, nTreeTopPts, nTrunkPts);
}

bool readScene(char* fName, Controller& c)
{
	std::ifstream is(fName);
	if (!is.good())
	{
		std::cerr << "Could not open " << fName << " for reading. Generating default scene.\n";
		return false;
	}
	std::string obj;
	while (is >> obj)
	{
		if (obj == "Mountain")
			c.addModel( readMountain(is) );
		else if (obj == "Tree")
			c.addModel( readTree(is) );
		else if (obj == "House")
			c.addModel( readHouse(is) );
		else
		{
			std::cerr << "Unrecognized object: " << obj << '\n';
			is.close();
			return false;
		}
	}
	is.close();
	return true;
}

static char* defaultFont = (char*)("../fontutil/fonts/LucidaCalligraphyItalic24.fnt");

int main(int argc, char* argv[])
{
	// One-time initialization of the glut
	glutInit(&argc, argv);

	Controller c("Mountain Village");
	char* useFont = NULL;
	bool haveScene = false;
	for (int i=1 ; i<argc ; i++)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == 'f')
				useFont = argv[++i];
		}
		else
		{
			if (useFont == NULL)
				useFont = defaultFont;
			House::setHouseLabelFont(useFont);
			haveScene = readScene(argv[i], c);
		}
	}
	if (!haveScene)
	{
		if (useFont == NULL)
			useFont = defaultFont;
		House::setHouseLabelFont(useFont);
		makeDefaultScene(c);
	}
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Off to the glut event handling loop:
	glutMainLoop();

	return 0;
}
