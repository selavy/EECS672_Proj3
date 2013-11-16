// Pawn.h

#ifndef PAWN_H
#define PAWN_H

#include "GeneralMV.h"
#include "Cylinder.h"
#include "Sphere.h"

#define PWN_CORNERS 16
#define PWN_FACES 12
#define PWN_VERTICES (6 * PWN_FACES)

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
  
  Cylinder * _midsection;
  Sphere * _top;
  int _color;
};

#endif
