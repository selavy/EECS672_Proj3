// Pawn.h

#ifndef PAWN_H
#define PAWN_H

#include "GeneralMV.h"

#define PWN_VERTICES 144
#define PWN_CORNERS 32
#define PWN_FACES 24

#define _V( x ) _vertices[ (x) ]

class Pawn : public GeneralMV
{
public:
  Pawn( int color, vec3 corner );
	virtual ~Pawn();

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
