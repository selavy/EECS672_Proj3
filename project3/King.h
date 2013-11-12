// King.h

#ifndef KING_H
#define KING_H

#include "GeneralMV.h"

#define KING_VERTICES 216
#define KING_CORNERS 48

#define _V( x ) _vertices[ (x) ]

class King : public GeneralMV
{
public:
  King( int color, vec3 corner );
	virtual ~King();

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
