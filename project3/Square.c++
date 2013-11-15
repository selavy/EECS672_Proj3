// Square.c++
#include "Square.h"

#include <iostream>
using namespace std;

Square::Square( 
	       int color,
	       vec3 corner,           //!> location of bottom left corner of square
	       float width,           //!> width of square
	       float thickness        //!> thickness of square
		)
  :
_color( color )
{
  _vertices = new vec3[SQ_CORNERS];

  _points = new vec3[ SQ_VERTICES ];
  _normals = new vec3[ SQ_VERTICES ];

  float xmin = corner[0];
  float xmax = corner[0] + width;
  float ymin = corner[1];
  float ymax = corner[1] + thickness;
  float zmin = corner[2];
  float zmax = corner[2] + width;

  /*
  float xmin = -0.5f;
  float xmax = 0.5f;
  float ymin = -0.5f;
  float ymax = 0.5f;
  float zmin = -0.5f;
  float zmax = 0.5f;
  */

  _limits[0] = xmin;
  _limits[1] = xmax;
  _limits[2] = ymin;
  _limits[3] = ymax;
  _limits[4] = zmin;
  _limits[5] = zmax;

  vec3 tmp[SQ_CORNERS] = {
    { xmin, ymin, zmin },
    { xmin, ymax, zmin },
    { xmax, ymax, zmin },
    { xmax, ymin, zmin },
    { xmin, ymin, zmax },
    { xmin, ymax, zmax },
    { xmax, ymax, zmax },
    { xmax, ymin, zmax }
  };

  memcpy( _vertices, tmp, SQ_CORNERS * sizeof( vec3 ) );

  defineModel();
} /* end Square::Square() */

Square::~Square()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  delete [] _points;
  delete [] _normals;
  delete [] _vertices;
} /* end Square::~Square */

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Square::getWCBoundingBox(double* xyzLimits) const
{
  memcpy( xyzLimits, _limits, 6 * sizeof( double ) );
} /* end Square::getWCBoundingBox() */

void Square::defineModel()
{
  int index = 0;

  quad( 1, 0, 3, 2, index );
  quad( 3, 0, 4, 7, index );
  quad( 2, 3, 7, 6, index );
  quad( 5, 4, 0, 1, index );
  quad( 6, 5, 1, 2, index );
  quad( 4, 5, 6, 7, index );

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SQ_VERTICES, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SQ_VERTICES, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcNormal );
} /* end Square::defineModel() */

void Square::render()
{
  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  
  glUseProgram( shaderProgram );
  
  getMatrices( _limits );

  vec4 kd;
  kd[3] = 1.0f;

  if( _color == WHITE )
    {
      kd[0] = RGB( 245.0f );
      kd[1] = RGB( 255.0f );
      kd[2] = RGB( 250.0f );
    }
  else if( _color == BLACK )
    {
      kd[0] = RGB( 100 );
      kd[1] = RGB( 69 );
      kd[2] = RGB( 19 );
    }
  else if( _color == BORDER )
    {
      kd[0] = RGB( 139.0f );
      kd[1] = RGB( 69.0f );
      kd[2] = RGB( 19.0f );
    }
  else if( _color == TEST )
    {
      kd[0] = 1.0f;
      kd[1] = 0.0f;
      kd[2] = 0.0f;
    }
  else
    {
      kd[0] = 0.0f;
      kd[1] = 0.0f;
      kd[2] = 1.0f;
    }

  vec4 ks = { 0.3, 0.3, 0.3, 1.0 };
  vec4 ka;
  memcpy( ka, kd, sizeof( vec4 ) );

  sendPhongLightModel( ka, kd, ks, 10 );

  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLES, 0, SQ_VERTICES );

  glUseProgram( pgm );
} /* end Square::render() */
