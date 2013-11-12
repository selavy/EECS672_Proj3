// Pawn.c++

#include "Pawn.h"
#include "ShaderIF.h"
#include "Controller.h"

#include <iostream>
using namespace std;

#define RGB(x) ( (x) / 255.0 )

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



#define BWIDTH 5.0f
#define MWIDTH 3.0f
#define TWIDTH 4.0f
#define PIN ((BWIDTH - MWIDTH) / 2.0f)
#define TIN ((BWIDTH - TWIDTH) / 2.0f)
#define BHEIGHT 1.8f
#define PHEIGHT 1.0f
#define MHEIGHT 2.5f
#define THEIGHT 1.5f

#define STARTP (BHEIGHT)
#define STARTM (BHEIGHT + PHEIGHT)
#define STARTT (STARTM + MHEIGHT)
#define ENDT (STARTT + THEIGHT)

#define PLEFTT (PIN)
#define PRGHTT (BWIDTH-PIN)
#define PLEFTB 0
#define PRGHTB (BWIDTH)
#define MLEFT (PIN)
#define MRGHT (BWIDTH-PIN)
#define TLEFT (TIN)
#define TRGHT (BWIDTH-TIN)

  // TODO
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

    // mid section
    { corner[0] + MLEFT, corner[1] + STARTM, corner[2] + MLEFT },
    { corner[0] + MLEFT, corner[1] + STARTT, corner[2] + MLEFT },
    { corner[0] + MRGHT, corner[1] + STARTT, corner[2] + MLEFT },
    { corner[0] + MRGHT, corner[1] + STARTM, corner[2] + MLEFT },
    { corner[0] + MLEFT, corner[1] + STARTM, corner[2] + MRGHT },
    { corner[0] + MLEFT, corner[1] + STARTT, corner[2] + MRGHT },
    { corner[0] + MRGHT, corner[1] + STARTT, corner[2] + MRGHT },
    { corner[0] + MRGHT, corner[1] + STARTM, corner[2] + MRGHT },

    // top
    { corner[0] + TLEFT, corner[1] + STARTT, corner[2] + TLEFT },
    { corner[0] + TLEFT, corner[1] + ENDT,   corner[2] + TLEFT },
    { corner[0] + TRGHT, corner[1] + ENDT,   corner[2] + TLEFT },
    { corner[0] + TRGHT, corner[1] + STARTT, corner[2] + TLEFT },
    { corner[0] + TLEFT, corner[1] + STARTT, corner[2] + TRGHT },
    { corner[0] + TLEFT, corner[1] + ENDT,   corner[2] + TRGHT },
    { corner[0] + TRGHT, corner[1] + ENDT,   corner[2] + TRGHT },
    { corner[0] + TRGHT, corner[1] + STARTT, corner[2] + TRGHT },

  };

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

  for( int i = 0; i < 25; i += 8 )
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

  vec4 color;
  color[3] = 1.0f;

#ifndef BLACK
#define BLACK 0
#endif

#ifndef WHITE
#define WHITE 1
#endif

  if( _color == BLACK )
    {
      color[0] = RGB( 49.0f );
      color[1] = RGB( 79.0f );
      color[2] = RGB( 79.0f );
    }
  else if( _color == WHITE )
    {
      color[0] = RGB( 245.0f );
      color[1] = RGB( 255.0f );
      color[2] = RGB( 250.0f );
    }
  else
    {
      color[0] = 0.0f;
      color[1] = 0.0f;
      color[2] = 1.0f;
    }

  sendLightSource( color );

  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLES, 0, PWN_VERTICES );

  glUseProgram( pgm );
} /* end Pawn::render() */
