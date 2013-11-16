// Bishop.c++

#include "Bishop.h"

#include <iostream>
using namespace std;

Bishop::Bishop(
	       int color,
	       vec3 corner //!> location of bottom left corner of square
	       )
  :
  _color( color )
{
  _vertices = new vec3[BISHOP_CORNERS];
  _points = new vec3[BISHOP_VERTICES];
  _normals = new vec3[BISHOP_VERTICES];

  const double BHEIGHT  = 2.0f;
  const double PHEIGHT  = 1.2f;
  const double MHEIGHT  = 4.5f;
  const double BPHEIGHT = 1.5f;
  const double TPHEIGHT = 2.8f;
  const double SHEIGHT  = 0.8f;
  
  const double STARTP  = BHEIGHT;
  const double STARTM  = BHEIGHT + PHEIGHT;
  const double STARTBP = STARTM + MHEIGHT;
  const double STARTTP = STARTBP + BPHEIGHT;
  const double STARTS  = STARTTP + TPHEIGHT;

  const double BWIDTH   = 5.0f;
  const double PWIDTHB  = BWIDTH;
  const double PWIDTHT  = 3.0f;
  const double MWIDTH   = 2.5f;
  const double BPWIDTHB = 2.5f;
  const double BPWIDTHT = 3.2f;
  const double TPWIDTHB = BPWIDTHT;
  const double TPWIDTHT = 0.8f;

  const double PLEFTB = 0;
  const double PRGHTB = (BWIDTH);
  const double PLEFTT = ((BWIDTH - MWIDTH) / 2.0f);
  const double PRGHTT = BWIDTH - PLEFTT;

  const double BPLEFTB = (BWIDTH - BPWIDTHB) / 2.0f;
  const double BPRGHTB = BWIDTH - BPLEFTB;
  const double BPLEFTT = (BWIDTH - BPWIDTHT) / 2.0f;
  const double BPRGHTT = BWIDTH - BPLEFTT;

  const double TPLEFTB = (BWIDTH - TPWIDTHB) / 2.0f;
  const double TPRGHTB = BWIDTH - TPLEFTB;
  const double TPLEFTT = (BWIDTH - TPWIDTHT) / 2.0f;
  const double TPRGHTT = BWIDTH - TPLEFTT;

  vec3 tmp[BISHOP_CORNERS] = {
    // base ==> 8
    { corner[0],          corner[1],           corner[2]          },
    { corner[0],          corner[1] + BHEIGHT, corner[2]          },
    { corner[0] + BWIDTH, corner[1] + BHEIGHT, corner[2]          },
    { corner[0] + BWIDTH, corner[1],           corner[2]          },
    { corner[0],          corner[1],           corner[2] + BWIDTH },
    { corner[0],          corner[1] + BHEIGHT, corner[2] + BWIDTH },
    { corner[0] + BWIDTH, corner[1] + BHEIGHT, corner[2] + BWIDTH },
    { corner[0] + BWIDTH, corner[1],           corner[2] + BWIDTH },
  
    // pyramid ==> 8
    { corner[0] + PLEFTB, corner[1] +  STARTP, corner[2]          },
    { corner[0] + PLEFTT, corner[1] +  STARTM, corner[2] + PLEFTT },
    { corner[0] + PRGHTT, corner[1] +  STARTM, corner[2] + PLEFTT },
    { corner[0] + PRGHTB, corner[1] +  STARTP, corner[2]          },
    { corner[0] + PLEFTB, corner[1] +  STARTP, corner[2] + PRGHTB },
    { corner[0] + PLEFTT, corner[1] +  STARTM, corner[2] + PRGHTT },
    { corner[0] + PRGHTT, corner[1] +  STARTM, corner[2] + PRGHTT },
    { corner[0] + PRGHTB, corner[1] +  STARTP, corner[2] + PRGHTB },
    
    // cylinder

    // head bottom pyramid ==> 8
    { corner[0] + BPLEFTB, corner[1] +  STARTBP, corner[2] + BPLEFTB },
    { corner[0] + BPLEFTT, corner[1] +  STARTTP, corner[2] + BPLEFTT },
    { corner[0] + BPRGHTT, corner[1] +  STARTTP, corner[2] + BPLEFTT },
    { corner[0] + BPRGHTB, corner[1] +  STARTBP, corner[2] + BPLEFTB },
    { corner[0] + BPLEFTB, corner[1] +  STARTBP, corner[2] + BPRGHTB },
    { corner[0] + BPLEFTT, corner[1] +  STARTTP, corner[2] + BPRGHTT },
    { corner[0] + BPRGHTT, corner[1] +  STARTTP, corner[2] + BPRGHTT },
    { corner[0] + BPRGHTB, corner[1] +  STARTBP, corner[2] + BPRGHTB },

    // head top pyramid ==> 8
    { corner[0] + TPLEFTB, corner[1] +  STARTTP, corner[2] + TPLEFTB },
    { corner[0] + TPLEFTT, corner[1] +  STARTS, corner[2] + TPLEFTT },
    { corner[0] + TPRGHTT, corner[1] +  STARTS, corner[2] + TPLEFTT },
    { corner[0] + TPRGHTB, corner[1] +  STARTTP, corner[2] + TPLEFTB },
    { corner[0] + TPLEFTB, corner[1] +  STARTTP, corner[2] + TPRGHTB },
    { corner[0] + TPLEFTT, corner[1] +  STARTS, corner[2] + TPRGHTT },
    { corner[0] + TPRGHTT, corner[1] +  STARTS, corner[2] + TPRGHTT },
    { corner[0] + TPRGHTB, corner[1] +  STARTTP, corner[2] + TPRGHTB }

    // sphere
  };

  memcpy( _vertices, tmp, BISHOP_CORNERS * sizeof( vec3 ) );

  vec3 cylinderloc;
  cylinderloc[0] = corner[0] + MWIDTH;
  cylinderloc[2] = corner[2] + MWIDTH;
  cylinderloc[1] = corner[1] + STARTM;

  _objs.push_back( new Cylinder( cylinderloc, 1.5, MHEIGHT, color ) );

  vec3 sphereloc;
  sphereloc[0] = corner[0] + MWIDTH;
  sphereloc[2] = corner[2] + MWIDTH;
  sphereloc[1] = corner[1] + STARTS;
  _objs.push_back( new Sphere( sphereloc, 0.8, color ) );

  _limits[0] = -1.0f;
  _limits[1] = 1.0f;
  _limits[2] = -1.0f;
  _limits[3] = 1.0f;
  _limits[4] = -1.0f;
  _limits[5] = 1.0f;

  defineModel();
} /* end Bishop::Bishop() */

Bishop::~Bishop()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );
  
  for( std::vector<GeneralMV*>::iterator it = _objs.begin(); it != _objs.end(); ++it )
    delete (*it);

  delete [] _points;
  delete [] _normals;
  delete [] _vertices;
} /* end Bishop::~Bishop() */

void Bishop::getWCBoundingBox( double* xyzLimitsF ) const
{
  memcpy( xyzLimitsF, _limits, 6 * sizeof( double ) );
} /* end Bishop::getWCBoundingBox() */

void Bishop::defineModel()
{
  int index = 0;
  for( int i = 0; i <= BISHOP_FACES; i += 8 )
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
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * BISHOP_VERTICES, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( Bishop::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Bishop::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * BISHOP_VERTICES, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( Bishop::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( Bishop::pvaLoc_wcNormal );
} /* end Bishop::defineModel() */

void Bishop::render()
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
  
  for( std::vector<GeneralMV*>::iterator it = _objs.begin(); it != _objs.end(); ++it )
    (*it)->render();

  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLES, 0, BISHOP_VERTICES );

  glUseProgram( pgm );
} /* end Bishop::render() */



