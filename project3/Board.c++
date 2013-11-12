#include "Board.h"

#define WHITE 1
#define BLACK 0
#define BORDER 2
#define FLIP( x ) do { (x) ^= 1; } while(0)

Board::Board( vec3 location, float width, float thickness )
{
  float start_x = location[0];

  // "White on right" --> black square on the left for a chess board
  int color = BLACK;
  for( int i = 0; i < 10; ++i )
    {
      for( int j = 0; j < 10; ++j )
	{
	  if( i == 0 || i == 9 || j == 0 || j == 9 )
	    {
	      // Border
	      _squares.push_back( new Square( BORDER, location, width, thickness ) );
	    }
	  else
	    {
	      // board square (checkered area)
	      _squares.push_back( new Square( color, location, width, thickness ) );
	    }

	  location[0] += width;
	  FLIP( color );
	}
      location[0] = start_x;
      location[2] += width;

      // have to flip twice on the edges to get the checker pattern
      FLIP( color ); 
    }
} /* end Board::Board() */

Board::~Board()
{
  for( std::vector<Square*>::iterator it = _squares.begin(); it != _squares.end(); ++it )
    delete (*it);
} /* end Board::~Board() */

void Board::getWCBoundingBox( double* xyzLimitsF ) const
{
  double xmin = 0;
  double xmax = 0;
  double ymin = 0;
  double ymax = 0;
  double zmin = 0;
  double zmax = 0;
  double tmp[6];

  for( std::vector<Square*>::const_iterator it = _squares.begin(); it != _squares.end(); ++it )
    {
      (*it)->getWCBoundingBox( tmp );

      if( tmp[0] < xmin )
	xmin = tmp[0];
      if( tmp[1] > xmax )
	xmax = tmp[1];
      if( tmp[2] < ymin )
	ymin = tmp[2];
      if( tmp[3] > ymax )
	ymax = tmp[3];
      if( tmp[4] < zmin )
	zmin = tmp[4];
      if( tmp[5] > zmax )
	zmax = tmp[5];
    }

  xyzLimitsF[0] = xmin;
  xyzLimitsF[1] = xmax;
  xyzLimitsF[2] = ymin;
  xyzLimitsF[3] = ymax;
  xyzLimitsF[4] = zmin;
  xyzLimitsF[5] = zmax;
} /* end Board::getWCBoundingBox() */

void Board::render()
{
  for( std::vector<Square*>::iterator it = _squares.begin(); it != _squares.end(); ++it )
    (*it)->render();
} /* end Board::render() */
