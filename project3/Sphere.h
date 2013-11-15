// Sphere.h

#ifndef SPHERE_H
#define SPHERE_H

#include <GL/gl.h>
#include "GeneralMV.h"

#define NumTimesToSubdivide 5
#define NumTriangles 4096
// (4 faces)^(NumTimesToSubdivide + 1)
#define NumVertices (3 * NumTriangles)

class Sphere : public GeneralMV
{
 public:
  Sphere( vec3 location, float radius, int color );
  virtual ~Sphere();
  
  void getWCBoundingBox( double* xyzLimitsF ) const;
  void defineSphere();
  void render();

 private:
  void tetrahedron( int count );
  void divide_triangle( const vec3& a, const vec3& b, const vec3& c, int count );
  
  GLuint vao;
  GLuint vbo[2]; //!> 0: coordinate data, 1: normals

  double _limits[6];
  int _Index;
  int _color;
  float _radius;
  vec3 _location;
};

#endif
