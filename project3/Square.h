// Square.h

#ifndef SQUARE_H
#define SQUARE_H

#include "GeneralMV.h"


#define SQ_VERTICES 36
#define SQ_CORNERS 8
#define SQ_FACES 6

#define _V( x ) _vertices[ (x) ]

class Square : public GeneralMV
{
public:
  Square( int color, vec3 corner, float width, float thickness );
	virtual ~Square();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void defineModel();
	void render();
	
private:
	GLuint vao;
	GLuint vbo[2]; //!> 0: coordinate data, 1: normals
	
	double _limits[6];
	int _color;

};

#endif
