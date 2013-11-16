// Pawn.c++

#include "Pawn.h"

#include <iostream>
using namespace std;

Pawn::Pawn( 
	       int color,
	       vec3 corner           //!> location of bottom left corner of square
	    )
  :
  _color( color )
{
  _vertices = new vec3[PWN_CORNERS];
  _points = new vec3[ PWN_VERTICES ];
  _normals = new vec3[ PWN_VERTICES ];

  const double BWIDTH = 5.0f;
  const double  MWIDTH = 3.0f;
  const double TWIDTH = 4.0f;
  const double PIN = ((BWIDTH - MWIDTH) / 2.0f);
  const double TIN = ((BWIDTH - TWIDTH) / 2.0f);
  const double BHEIGHT = 1.8f;
  const double PHEIGHT = 1.0f;
  const double MHEIGHT = 2.5f;
  const double THEIGHT = 1.0f;

  const double STARTP = (BHEIGHT);
  const double STARTM = (BHEIGHT + PHEIGHT);
  const double STARTT = (STARTM + MHEIGHT);
  const double ENDT = (STARTT + THEIGHT);

  const double PLEFTT = (PIN);
  const double PRGHTT = (BWIDTH-PIN);
  const double PLEFTB = 0;
  const double PRGHTB = (BWIDTH);
  const double MLEFT = (PIN);
  const double MRGHT = (BWIDTH-PIN);
  const double TLEFT = (TIN);
  const double TRGHT = (BWIDTH-TIN);

  // TODO ** DONE **
  vec3 tmp[PWN_CORNERS] = {
    // base
    { corner[0],          corner[1],           corner[2]          },
    { corner[0],          corner[1] + BHEIGHT, corner[2]          },
    { corner[0] + BWIDTH, corner[1] + BHEIGHT, corner[2]          },
    { corner[0] + BWIDTH, corner[1],           corner[2]          },
    { corner[0],          corner[1],           corner[2] + BWIDTH },
    { corner[0],          corner[1] + BHEIGHT, corner[2] + BWIDTH },
    { corner[0] + BWIDTH, corner[1] + BHEIGHT, corner[2] + BWIDTH },
    { corner[0] + BWIDTH, corner[1],           corner[2] + BWIDTH },
  
    // pyramid
    { corner[0] + PLEFTB, corner[1] +  STARTP, corner[2]          },
    { corner[0] + PLEFTT, corner[1] +  STARTM, corner[2] + PLEFTT },
    { corner[0] + PRGHTT, corner[1] +  STARTM, corner[2] + PLEFTT },
    { corner[0] + PRGHTB, corner[1] +  STARTP, corner[2]          },
    { corner[0] + PLEFTB, corner[1] +  STARTP, corner[2] + PRGHTB },
    { corner[0] + PLEFTT, corner[1] +  STARTM, corner[2] + PRGHTT },
    { corner[0] + PRGHTT, corner[1] +  STARTM, corner[2] + PRGHTT },
    { corner[0] + PRGHTB, corner[1] +  STARTP, corner[2] + PRGHTB },

  };

  vec3 cylindercorner;
  cylindercorner[0] = corner[0] + MWIDTH / 1.2f;
  cylindercorner[2] = corner[2] + MWIDTH / 1.2f;
  cylindercorner[1] = corner[1] + MHEIGHT;

  _midsection = new Cylinder( cylindercorner, 1.5, 2.5, color );

  vec3 topcorner;
  topcorner[0] = corner[0] + MWIDTH / 1.2f;
  topcorner[2] = corner[2] + MWIDTH / 1.2f;
  topcorner[1] = corner[1] + STARTT + THEIGHT;
  _top = new Sphere( topcorner, 2.2, color ); 

  memcpy( _vertices, tmp, PWN_CORNERS * sizeof( vec3 ) );

  _limits[0] = corner[0];
  _limits[1] = corner[0] + BWIDTH;
  _limits[2] = corner[1];
  _limits[3] = corner[1] + ENDT;
  _limits[4] = corner[2];
  _limits[5] = corner[2] + BWIDTH;
  
  defineModel();
} /* end Pawn::Pawn() */

Pawn::~Pawn()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  delete _midsection;
  delete _top;

  delete [] _points;
  delete [] _normals;
  delete [] _vertices;
} /* end Pawn::~Pawn */

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Pawn::getWCBoundingBox(double* xyzLimits) const
{
  memcpy( xyzLimits, _limits, 6 * sizeof( double ) );
} /* end Pawn::getWCBoundingBox() */

void Pawn::defineModel()
{
  int index = 0;

  for( int i = 0; i <= PWN_FACES; i += 8 )
    {
      quad( i+1, i, i+3, i+2, index );
      quad( i+3, i, i+4, i+7, index );
      quad( i+2, i+3, i+7, i+6, index );
      quad( i+5, i+4, i, i+1, index );
      quad( i+6, i+5, i+1, i+2, index );
      quad( i+4, i+5, i+6, i+7, index );
    }

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * PWN_VERTICES, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( Pawn::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Pawn::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * PWN_VERTICES, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( Pawn::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( Pawn::pvaLoc_wcNormal );
} /* end Pawn::defineModel() */

void Pawn::render()
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

  vec4 ks = { 0.5, 0.5, 0.5, 1.0 };
  vec4 ka;
  memcpy( ka, kd, sizeof( vec4 ) );

  sendPhongLightModel( ka, kd, ks, 30 );

  _midsection->render();
  _top->render();

  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLES, 0, PWN_VERTICES );

  glUseProgram( pgm );
} /* end Pawn::render() */
