#include <cmath>

#include "Sphere.h"
#include <iostream>

using namespace std;

Sphere::Sphere(
		   vec3 location,
		   float radius,
		   int color
		    )
  : 
  _Index( 0 ),
  _color( color ),
  _radius( radius )
{
  _points = new vec3[NumVertices];
  _normals = new vec3[NumVertices];

  memcpy( _location, location, sizeof( vec3 ) );

  /*
  _limits[0] = _location[0] + -1.0 * _radius;
  _limits[1] = _location[0] + 1.0 * _radius;
  _limits[2] = _location[1] + -1.0f * (_height / 2.0);
  _limits[3] = _location[1] + 1.0 * (_height / 2.0);
  _limits[4] = _location[2] + -1.0 * _radius;
  _limits[5] = _location[2] + 1.0 * _radius;
  */
  _limits[0] = -1.0;
  _limits[1] = 1.0;
  _limits[2] = -1.0;
  _limits[3] = 1.0;
  _limits[4] = -1.0;
  _limits[5] = 1.0;

  defineSphere();
}

Sphere::~Sphere()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  delete [] _points;
  delete [] _normals;
}

void Sphere::defineSphere()
{
  _Index = 0;
  tetrahedron( NumTimesToSubdivide );

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * NumVertices, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * NumVertices, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( pvaLoc_wcNormal );
} /* end Sphere::defineSphere() */

void Sphere::getWCBoundingBox( double* xyzLimitsF ) const
{
  memcpy( xyzLimitsF, _limits, 6 * sizeof( double ) );
} /* end Sphere::getWCBoundingBox() */

void Sphere::render()
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
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  glUseProgram( pgm );
}

void Sphere::tetrahedron( int count )
{
  /* copied from "Interactive Computer Graphics" by E. Angel, p. 633 */
  vec3 v[4] = {
    { 0.0, 0.0, 1.0 },
    { 0.0, 0.942809, -0.333333 },
    { -0.816497, -0.471405, -0.333333 },
    { 0.816497, -0.471405, -0.333333 }
  };

  divide_triangle( v[0], v[1], v[2], count );
  divide_triangle( v[3], v[2], v[1], count );
  divide_triangle( v[0], v[3], v[1], count );
  divide_triangle( v[0], v[2], v[3], count );
} /* end Sphere::tetrahedron() */

void Sphere::divide_triangle( const vec3& a, const vec3& b, const vec3& c, int count )
{
  /* copied from "Interactive Computer Graphics" by E. Angel, p. 633 */
  if( count > 0 )
    {
      cryph::AffVector tv1 = cryph::AffVector( a );
      tv1 += cryph::AffVector( b );
      tv1.normalize();
      
      cryph::AffVector tv2 = cryph::AffVector( a );
      tv2 += cryph::AffVector( c );
      tv2.normalize();

      cryph::AffVector tv3 = cryph::AffVector( b );
      tv3 += cryph::AffVector( c );
      tv3.normalize();

      vec3 v1, v2, v3;
      tv1.vComponents( v1 );
      tv2.vComponents( v2 );
      tv3.vComponents( v3 );

      divide_triangle( a, v1, v2, count - 1 );
      divide_triangle( c, v2, v3, count - 1 );
      divide_triangle( b, v3, v1, count - 1 );
      divide_triangle( v1, v3, v2, count - 1 );
    }
  else
    {
      vec3 aa, bb, cc;
      for( short i = 0; i < 3; ++i )
	{
	  aa[i] = a[i] * _radius + _location[i];
	  bb[i] = b[i] * _radius + _location[i];
	  cc[i] = c[i] * _radius + _location[i];
	}
      tri( aa, bb, cc, _Index );
    }
} /* end Sphere::divide_triangle() */
