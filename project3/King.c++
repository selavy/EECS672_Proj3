// King.c++

#include "King.h"
#include "ShaderIF.h"
#include "Controller.h"

#include <iostream>
using namespace std;

#define RGB(x) ( (x) / 255.0 )

King::King( 
	       int color,
	       vec3 corner           //!> location of bottom left corner of square
	    )
  :
  _color( color )
{
  _vertices = new vec3[ KING_CORNERS];
  _points = new vec3[ KING_VERTICES ];
  _normals = new vec3[ KING_VERTICES ];

#define BW 6.5f
#define MW 2.5f
#define RW 3.75f
#define TW 5.0f
#define CW 0.5f
#define HW 2.0f

#define CD 0.7f

#define BH 3.5f
#define MH 6.0f
#define RH 0.25f
#define TH 2.5f
#define CH 2.0f
#define HH 0.5f

#define SEP 0.5f

#define BB (0)
#define BT (BH)
#define MB (BH)
#define MT (MB+MH)
#define RB (MT)
#define RT (RB+RH)
#define TB (RT)
#define TT (TB+TH)
#define CB (TT)
#define CT (CB+CH)
#define HB (CB+SEP)
#define HT (HB+HH)

#define BLB (0)
#define BRB (BW)
#define BLT ((BW-MW) / 2.0f)
#define BRT (BLT+MW)
#define MLB (BLT)
#define MRB (BRT)
#define MLT (MLB)
#define MRT (MRB)
#define RLB ((BW-RW) / 2.0f)
#define RRB (RLB+RW)
#define RLT (RLB)
#define RRT (RRB)
#define TLB (MLT)
#define TRB (MRT)
#define TLT ((BW-TW) / 2.0f)
#define TRT (TLT+TW)
#define CLB ((BW-CW) / 2.0f)
#define CRB (CLB+CW)
#define CLT (CLB)
#define CRT (CRB)
#define HLB ((BW-HW) / 2.0f)
#define HRB (HLB+HW)
#define HLT (HLB)
#define HRT (HRB)

#define KINGHEIGHT (BH + MH + RH + TH + CH)


  // TODO
  vec3 tmp[KING_CORNERS] = {
    // base
    { corner[0] + BLB, corner[1] + BB, corner[2]       },
    { corner[0] + BLT, corner[1] + BT, corner[2] + BLT },
    { corner[0] + BRT, corner[1] + BT, corner[2] + BLT },
    { corner[0] + BRB, corner[1] + BB, corner[2]       },
    { corner[0] + BLB, corner[1] + BB, corner[2] + BRB },
    { corner[0] + BLT, corner[1] + BH, corner[2] + BRT },
    { corner[0] + BRT, corner[1] + BH, corner[2] + BRT },
    { corner[0] + BRB, corner[1] + BB, corner[2] + BRB },
  
    // middle
    { corner[0] + MLB, corner[1] + MB, corner[2] + MLB },
    { corner[0] + MLT, corner[1] + MT, corner[2] + MLT },
    { corner[0] + MRT, corner[1] + MT, corner[2] + MLT },
    { corner[0] + MRB, corner[1] + MB, corner[2] + MLB },
    { corner[0] + MLB, corner[1] + MB, corner[2] + MRB },
    { corner[0] + MLT, corner[1] + MT, corner[2] + MRT },
    { corner[0] + MRT, corner[1] + MT, corner[2] + MRT },
    { corner[0] + MRB, corner[1] + MB, corner[2] + MRB },

    // ring
    { corner[0] + RLB, corner[1] + RB, corner[2] + RLB },
    { corner[0] + RLT, corner[1] + RT, corner[2] + RLT },
    { corner[0] + RRT, corner[1] + RT, corner[2] + RLT },
    { corner[0] + RRB, corner[1] + RB, corner[2] + RLB },
    { corner[0] + RLB, corner[1] + RB, corner[2] + RRB },
    { corner[0] + RLT, corner[1] + RT, corner[2] + RRT },
    { corner[0] + RRT, corner[1] + RT, corner[2] + RRT },
    { corner[0] + RRB, corner[1] + RB, corner[2] + RRB },

    // top
    { corner[0] + TLB, corner[1] + TB, corner[2] + TLB },
    { corner[0] + TLT, corner[1] + TT, corner[2] + TLT },
    { corner[0] + TRT, corner[1] + TT, corner[2] + TLT },
    { corner[0] + TRB, corner[1] + TB, corner[2] + TLB },
    { corner[0] + TLB, corner[1] + TB, corner[2] + TRB },
    { corner[0] + TLT, corner[1] + TT, corner[2] + TRT },
    { corner[0] + TRT, corner[1] + TT, corner[2] + TRT },
    { corner[0] + TRB, corner[1] + TB, corner[2] + TRB },

    // vertical cross
    { corner[0] + CLB, corner[1] + CB, corner[2] + CLB },
    { corner[0] + CLT, corner[1] + CT, corner[2] + CLT },
    { corner[0] + CRT, corner[1] + CT, corner[2] + CLT },
    { corner[0] + CRB, corner[1] + CB, corner[2] + CLB },
    { corner[0] + CLB, corner[1] + CB, corner[2] + CRB },
    { corner[0] + CLT, corner[1] + CT, corner[2] + CRT },
    { corner[0] + CRT, corner[1] + CT, corner[2] + CRT },
    { corner[0] + CRB, corner[1] + CB, corner[2] + CRB },

    // horizontal cross
    { corner[0] + HLB + CD, corner[1] + HB, corner[2] + HLB },
    { corner[0] + HLT + CD, corner[1] + HT, corner[2] + HLT },
    { corner[0] + HRT - CD, corner[1] + HT, corner[2] + HLT },
    { corner[0] + HRB - CD, corner[1] + HB, corner[2] + HLB },
    { corner[0] + HLB + CD, corner[1] + HB, corner[2] + HRB },
    { corner[0] + HLT + CD, corner[1] + HT, corner[2] + HRT },
    { corner[0] + HRT - CD, corner[1] + HT, corner[2] + HRT },
    { corner[0] + HRB - CD, corner[1] + HB, corner[2] + HRB },

  };

  memcpy( _vertices, tmp, KING_CORNERS * sizeof( vec3 ) );

#ifndef PAD
#define PAD 5.0f
#endif

  _limits[0] = corner[0] - PAD;
  _limits[1] = corner[0] + BW + PAD;
  _limits[2] = corner[1] - PAD;
  _limits[3] = corner[1] + KINGHEIGHT + PAD;
  _limits[4] = corner[2] - PAD;
  _limits[5] = corner[2] + BW + PAD;
  
  defineModel();
} /* end King::King() */

King::~King()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  delete [] _points;
  delete [] _normals;
  delete [] _vertices;
} /* end King::~King */

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void King::getWCBoundingBox(double* xyzLimits) const
{
  memcpy( xyzLimits, _limits, 6 * sizeof( double ) );
} /* end King::getWCBoundingBox() */

void King::defineModel()
{
  int index = 0;

  for( int i = 0; i < KING_CORNERS; i += 8 )
    {
      quad( i+1, i,   i+3, i+2, index );
      quad( i+3, i,   i+4, i+7, index );
      quad( i+2, i+3, i+7, i+6, index );
      quad( i+5, i+4, i,   i+1, index );
      quad( i+6, i+5, i+1, i+2, index );
      quad( i+4, i+5, i+6, i+7, index );
    }

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * KING_VERTICES, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( King::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( King::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * KING_VERTICES, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( King::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( King::pvaLoc_wcNormal );
} /* end King::defineModel() */

void King::render()
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
  glDrawArrays( GL_TRIANGLES, 0, KING_VERTICES );

  glUseProgram( pgm );
} /* end King::render() */
