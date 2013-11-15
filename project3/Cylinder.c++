#include <cmath>

#include "Cylinder.h"
#include <iostream>

using namespace std;

#define RGB(x) ( (x) / 255.0 )

Cylinder::Cylinder(
		   vec3 location,
		   float radius,
		   float height,
		   int color
		    )
  : 
  _color( color ),
  _radius( radius ),
  _height( height )
{
  memcpy( _location, location, sizeof( vec3 ) );

  _limits[0] = _location[0] + -1.0 * _radius;
  _limits[1] = _location[0] + 1.0 * _radius;
  _limits[2] = _location[1] + -1.0f * (_height / 2.0);
  _limits[3] = _location[1] + 1.0 * (_height / 2.0);
  _limits[4] = _location[2] + -1.0 * _radius;
  _limits[5] = _location[2] + 1.0 * _radius;

  defineCylinder();
}

Cylinder::~Cylinder()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );
}

const int N_POINTS_AROUND_SLICE = 18; // number of points around a cross-section

void Cylinder::defineCylinder()
{
  int nPoints = 2 * (N_POINTS_AROUND_SLICE + 1);
  vec3 * coords = new vec3[nPoints];
  vec3 * normals = new vec3[nPoints];
  double theta = 0.0;
  double dTheta = 2.0*M_PI/N_POINTS_AROUND_SLICE;
  //double vb = -0.25, zb = 0.45;
  double vb = _location[2];
  double zb = _location[0];
  double z1 = _location[1];
  double z2 = _location[1] + _height;
  double r = _radius;

  for( int i = 0; i <= N_POINTS_AROUND_SLICE; ++i )
    {
      int j = 2 * i;
      double dx = cos( theta );
      double dz = sin( theta );

      normals[j][0] = dx; normals[j][1] = 0.0; normals[j][2] = dz;
      coords[j][0] = zb + r*dx; coords[j][1] = z1; coords[j][2] = vb + r*dz;
      normals[j+1][0] = dx; normals[j+1][1] = 0.0; normals[j+1][2] = dz;
      coords[j+1][0] = zb + r*dx; coords[j+1][1] = z2; coords[j+1][2] = vb + r*dz;
      theta += dTheta;
    }

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * nPoints, coords, GL_STATIC_DRAW );
  glVertexAttribPointer( pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * nPoints, normals, GL_STATIC_DRAW );
  glVertexAttribPointer( pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( pvaLoc_wcNormal );

  delete [] coords;
  delete [] normals;
} /* end Cylinder::defineCylinder() */

void Cylinder::getWCBoundingBox( double* xyzLimitsF ) const
{
  memcpy( xyzLimitsF, _limits, 6 * sizeof( double ) );
} /* end Cylinder::getWCBoundingBox() */

void Cylinder::render()
{
  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  glUseProgram( shaderProgram );
  getMatrices( _limits );

  vec4 kd;
  kd[3] = 1.0f;

#ifndef BLACK
#define BLACK 0
#endif

#ifndef WHITE
#define WHITE 1
#endif

  if( _color == BLACK )
    {
      kd[0] = RGB( 49.0f );
      kd[1] = RGB( 79.0f );
      kd[2] = RGB( 79.0f );
    }
  else if( _color == WHITE )
    {
      kd[0] = RGB( 245.0f );
      kd[1] = RGB( 255.0f );
      kd[2] = RGB( 250.0f );
    }
  else
    {
      kd[0] = 0.0f;
      kd[1] = 0.0f;
      kd[2] = 1.0f;
    }

  vec4 ks = { 0.1, 0.1, 0.1, 1.0 };
  vec4 ka;
  memcpy( ka, kd, sizeof( vec4 ) );
  sendPhongLightModel( ka, kd, ks, 25 );
  
  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));

  glUseProgram( pgm );
}
