// Cylinder.h

#ifndef CYLINDER_H
#define CYLINDER_H

#include <GL/gl.h>
#include "GeneralMV.h"

class Cylinder : public GeneralMV
{
 public:
  Cylinder( vec3 location, float radius, float height, int color );
  virtual ~Cylinder();
  
  void getWCBoundingBox( double* xyzLimitsF ) const;
  void defineCylinder();
  void render();

 private:
  GLuint vao;
  GLuint vbo[2]; //!> 0: coordinate data, 1: normals

  double _limits[6];
  
  int _color;
  float _radius;
  float _height;
  vec3 _location;
};

#endif
