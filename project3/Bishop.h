// Bishop.h

#ifndef BISHOP_H
#define BISHOP_H

#include "GeneralMV.h"
#include "Cylinder.h"
#include "Sphere.h"

#include <vector>

#define BISHOP_CORNERS 32
#define BISHOP_FACES 24
#define BISHOP_VERTICES (6 * BISHOP_FACES)

class Bishop : public GeneralMV
{
 public:
  Bishop( int color, vec3 corner );
  virtual ~Bishop();

  // xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax }
  void getWCBoundingBox( double* xyzLimitsF ) const;
  void defineModel();
  void render();

 private:
  GLuint vao;
  GLuint vbo[2]; //!> 0: coordinate data, 1: normals
  
  double _limits[6];

  std::vector<GeneralMV*> _objs;

  int _color;
};

#endif
