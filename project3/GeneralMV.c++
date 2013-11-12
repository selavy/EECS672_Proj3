// GeneralMV.c++

#include "GeneralMV.h"

using namespace std;

#define __DEBUG__

#define SQRD( x ) ( x * x )

#define ZEYEMULTIPLIER ( 5.0f )
#define _MULTIPLIER ( 1.8f )
#define MOVSPEED ( 0.1f )

/* static */ ShaderIF * GeneralMV::shaderIF = NULL;
/* static */ int GeneralMV::numInstances = 0;
/* static */ GLuint GeneralMV::shaderProgram = 0;

/* static */ GLint GeneralMV::ppuLoc_M4x4_wc_ec = -1;
/* static */ GLint GeneralMV::ppuLoc_M4x4_ec_lds = -1;
/* static */ GLint GeneralMV::ppuLoc_lightModel = -1;
/* static */ GLint GeneralMV::ppuLoc_kd = -1;
/* static */ GLint GeneralMV::pvaLoc_wcPosition = -1;
/* static */ GLint GeneralMV::pvaLoc_wcNormal = -1;

/* static */ vec3 GeneralMV::_eye = { 0.0f, 0.0f, 1.0f };

/* static */ vec3 GeneralMV::_up = { 0.0f, 1.0f, 0.0f }; // always up will be the y-axis
/* static */ vec3 GeneralMV::_center = { 0.0f, 0.0f, 0.0f }; // for now just the origin

// TODO: (high priority)
// Add 2 more light sources
// TODO: (high priority)
// Implement full Phong Lighting model
/* static */ vec3 GeneralMV::_lightloc = { 0.0f, 0.0f, 1.0f }; // for now directly from the eye location

/* static */ vec3 GeneralMV::_ecmin = { -1.0, -1.0f, -1.0f };
/* static */ vec3 GeneralMV::_ecmax = {  1.0f, 1.0f,  1.0f };

// TODO:
// Change this to perspective (high priority)
/* static */ GeneralMV::PROJECTION_TYPE GeneralMV::_proj_type = ORTHOGONAL;
/* static */ float GeneralMV::_r = 0.0f;


GeneralMV::GeneralMV()
{
  if( GeneralMV::shaderProgram == 0 )
    {
      GeneralMV::shaderIF = new ShaderIF( "simple3d.vsh", "simple3d.fsh" );
      GeneralMV::shaderProgram = shaderIF->getShaderPgmID();

      fetchGLSLVariableLocations();
    }
} /* end GeneralMV::GeneralMV() */

GeneralMV::~GeneralMV()
{
  // since this is the base class
  // the destructor of the derived class
  // will be called first.
  // so it is the responsibility of the
  // derived class to delete
  // its vao and vbo variables
  if( --GeneralMV::numInstances == 0 )
    {
      GeneralMV::shaderIF->destroy();
      delete GeneralMV::shaderIF;
      GeneralMV::shaderIF = NULL;
      GeneralMV::shaderProgram = 0;
    }
} /* end GeneralMV::~GeneralMV() */

void GeneralMV::printEyeLoc()
{
#ifdef __DEBUG__
  cout << "Eye location: (" << _eye[0] << "," << _eye[1] << "," << _eye[2] << ")" << endl;
#endif
}

void GeneralMV::printBox()
{
#ifdef __DEBUG__
  cout << "Box:" << endl;
  cout << "x: (" << _ecmin[0] << "," << _ecmax[0] << ")" << endl;
  cout << "y: (" << _ecmin[1] << "," << _ecmax[1] << ")" << endl;
  cout << "z: (" << _ecmin[2] << "," << _ecmax[2] << ")" << endl;
#endif
}

void GeneralMV::handleCommand( unsigned char key, double ldsX, double ldsY )
{
// TODO: (med priority)
// must calculate a bounding sphere and update the projection matrix
// when the screen is moved.

// TODO: (low priority)
// standardize key movements
// w,a,s,d ==> up, left, down, right
// z, x ==> zoom in, zoom out
// w,a,s,d should move spherically (i.e. as if defined in spherical coordinates,
// should rotate around the image, not just pan horizontally, vertically
// implement min and max for zoom

  if( key == 'q' )
    {
      _proj_type = OBLIQUE;
    }
  else if( key == 'o' )
    {
      _proj_type = ORTHOGONAL;
    }
  else if( key == 'p' )
    {
      _proj_type = PERSPECTIVE;
    }
  
  if( key == 'w' )
    {
      _eye[2] += MOVSPEED;
    }
  else if( key == 's' )
    {
      _eye[2] -= MOVSPEED;
    }
  else if( key == 'a' )
    {
      _eye[0] -= MOVSPEED;
    }
  else if( key == 'd' )
    {
      _eye[0] += MOVSPEED;
    }
  else if( key == 'r' )
    {
      _eye[1] += MOVSPEED;
    }
  else if( key == 'f' )
    {
      _eye[1] -= MOVSPEED;
    }
  else if( key == 'o' )
    {
_eye[0] = 0.0f;
_eye[1] = 0.0f;
_eye[2] = -1.0f * GeneralMV::_r * ZEYEMULTIPLIER;
    }
  else if( key == 'i' )
    {
      _ecmin[2] += MOVSPEED;
      _ecmax[2] += MOVSPEED;
    }
  else if( key == 'k' )
    {
      _ecmin[2] -= MOVSPEED;
      _ecmax[2] -= MOVSPEED;
    }
  else if( key == 'j' )
    {
      _ecmin[0] -= MOVSPEED;
      _ecmax[0] -= MOVSPEED;
    }
  else if( key == 'l' )
    {
      _ecmin[0] += MOVSPEED;
      _ecmax[0] += MOVSPEED;
    }
  else if( key == 'y' )
    {
      _ecmin[1] -= MOVSPEED;
      _ecmax[1] -= MOVSPEED;
    }
  else if( key == 'u' )
    {
      _ecmin[1] += MOVSPEED;
      _ecmax[1] += MOVSPEED;
    }

  else if( key == 'z' )
    {
      // TODO: (med priority)
      // zoom by moving frustum in _perspective
      _ecmax[2] += MOVSPEED;
    }

  else if( key == 'n' )
    {
      _ecmin[2] -= MOVSPEED;
    }

  printEyeLoc();
  printBox();
} /* end GeneralMV::handleCommand() */

void GeneralMV::calcBoundingSphere()
{
  // get the region of interest from the controller
  double xyzLimits[6];
  Controller::getCurrentController()->getWCRegionOfInterest( xyzLimits );

#ifdef __DEBUG__
  cout << "REGION OF INTEREST: " << endl;
  for( short i = 0; i < 6; i += 2 )
    cout << "(" << xyzLimits[i] << ", " << xyzLimits[i+1] << ")" << endl;
#endif

  // compute C (midpoint of region of interest)
  _center[0] = ( xyzLimits[1] + xyzLimits[0] ) / 2.0f;
  _center[1] = ( xyzLimits[3] + xyzLimits[2] ) / 2.0f;
  _center[2] = ( xyzLimits[5] + xyzLimits[4] ) / 2.0f;

  double xdist, ydist, zdist;

  // find the distance ( sqrt[x^2 + y^2 + z^2] ) to the farthest corner
  // save expensive sqrt operation for the end

  // first to the (xmin, ymin, zmin)
  xdist = xyzLimits[0] - _center[0];
  ydist = xyzLimits[2] - _center[1];
  zdist = xyzLimits[4] - _center[2];
  double tmpmin = SQRD( xdist ) + SQRD( ydist ) + SQRD( zdist );

  // second to the (xmax, ymax, zmax)
  xdist = xyzLimits[1] - _center[0];
  ydist = xyzLimits[3] - _center[1];
  zdist = xyzLimits[5] - _center[2];
  double tmpmax = SQRD( xdist ) + SQRD( ydist ) + SQRD( zdist );

  // TODO: (med priority)
  // check other corners

  // TODO: (lowest priority)
  // get a faster sqrt function
  _r = (tmpmin > tmpmax) ? sqrt( tmpmin ) : sqrt( tmpmax );

  // e = normalize( < 3, 1, 5 > )
#define _EX ( 0.507093f )
#define _EY ( 0.169030f )
#define _EZ ( 0.845154f )
#define _F ( 3.0f )
#define _D ( _F * _r )

  _eye[0] = _center[0] + _D * _EX;
  _eye[1] = _center[1] + _D * _EY;
  _eye[2] = _center[2] + _D * _EZ;
} /* end GeneralMV::calcBoundingSphere() */

void GeneralMV::setECMinMax()
{
  calcBoundingSphere();
  // start with (-r, r, -r, r) for (xmin, xmax, ymin, ymax)
  _ecmin[0] = -1.0f * _r;
  _ecmin[1] = -1.0f * _r;
  _ecmax[0] = GeneralMV::_r;
  _ecmax[1] = GeneralMV::_r;

  // then adjust to keep viewport aspect ratio the same
  // VAR = screen_y / screen_x
  double VAR = Controller::getCurrentController()->getViewportAspectRatio();

  // if VAR > 1 then increase y
  if( VAR > 1.0f )
    {
      _ecmin[1] *= VAR;
      _ecmax[1] *= VAR;
    }
  // if VAR < 1 then increase x
  else
    {
      _ecmin[0] /= VAR;
      _ecmax[0] /= VAR;
    }

  // the tricky part: set the z-axis
  _ecmax[2] = _r - _D;
  _ecmin[2] = -1.0f * (_D + _r);

#ifdef __DEBUG__
  cout << "CENTER:" << endl;
  for( short i = 0; i < 3; ++i )
    cout << _center[i] << endl;
  
  cout << "_r = " << _r << endl;
  cout << "VAR = " << VAR << endl;
  cout << "adjusted ratio = " << (_ecmax[1] - _ecmin[1])/(_ecmax[0] - _ecmin[0]) << endl;
  
  for( short i = 0; i < 3; ++i )
    cout << "(" << _ecmin[i] << ", " << _ecmax[i] << ")" << endl;
#endif

} /* end GeneralMV::setECMinMax() */

void GeneralMV::getMatrices( double limits[6] )
{
  //   I think there should be a model matrix created to move
  //   from model coordinates to world coordinates, but that
  //   doesn't seem to be facilitated here. So the models
  //   are just created in world coordinates

  // set the Eye Coordinate minimums and maximums using a bounding sphere
  setECMinMax();
  printEyeLoc();

  // Get the View matrix, Model matrix is I since already in WC
  // this matrix does move from wc -> ec                        
  wcToECMatrix(
	       _eye[0], _eye[1], _eye[2],
	       _center[0], _center[1], _center[2],
	       _up[0], _up[1], _up[2],
	       _model_view
	       );

  // TODO: (high priority)
  // Generate _ecmin, _ecmax based upon bounding sphere

  // TODO: (high priority)
  // Change the projection matrix based upon _proj_type
  // if( _proj_type == ORTHOGONAL )
  //  {
  //Get the orthogonal projection matrix
  orthogonal(
  	     _ecmin[0], _ecmax[0], _ecmin[1],
  	     _ecmax[1], _ecmin[2], _ecmax[2],
  	     _projection
  	     );
  
  // }
  // else if( _proj_type == OBLIQUE )
  //  {
  //     oblique( zpp, xmin, xmax, ymin, ymax, zmin, zmax, dx, dy, dz, m[] ); length(m) == 16
  //  }
  // else( _proj_type == PERSPECTIVE )
  //  {
  //	  perspective( zpp, xmin, xmax, ymin, ymax, zmin, zmax, m[] ); length(m) == 16
  //  }
  //
  
  glUniformMatrix4fv( GeneralMV::ppuLoc_M4x4_wc_ec, 1, GL_FALSE, _model_view );
  glUniformMatrix4fv( GeneralMV::ppuLoc_M4x4_ec_lds, 1, GL_FALSE, _projection );
} /* end GeneralMV::getMatrices() */

/**
 * The lighting model for this project
 * is a very simple diffusive light model.
 */
void GeneralMV::sendLightSource(
				vec4 kd    //!> The base color of the object
				)
{
  // TODO: (high priority)
  // Implement full Phong Lighting model with 3 light sources (2 direction, 1 positional)
  glUniform4fv( GeneralMV::ppuLoc_kd, 1, kd );
  glUniform3fv( GeneralMV::ppuLoc_lightModel, 1, _lightloc );
} /* end GeneralMV::sendLightSource() */

void GeneralMV::fetchGLSLVariableLocations()
{
  if( GeneralMV::shaderProgram > 0 )
    {
      GeneralMV::ppuLoc_M4x4_wc_ec = ppUniformLocation( shaderProgram, "M4x4_wc_ec" );
      GeneralMV::ppuLoc_M4x4_ec_lds = ppUniformLocation( shaderProgram, "M4x4_ec_lds" );
      GeneralMV::ppuLoc_kd = ppUniformLocation( shaderProgram, "kd" );
      GeneralMV::ppuLoc_lightModel = ppUniformLocation( shaderProgram, "lightModel" );
      GeneralMV::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      GeneralMV::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
    }

} /* end GeneralMV::fetchGLSLVariableLocations() */

void GeneralMV::tri( const vec3& veca,
		     const vec3& vecb,
		     const vec3& vecc,
		     int& Index           //!> The index in _points and _normals at which to start
		     )
{
  cryph::AffVector a( veca );
  cryph::AffVector b( vecb );
  cryph::AffVector c( vecc );

  /* based on the example function quad() from Interactive Computer Graphics by E. Angel, p. 627 */
  cryph::AffVector normal = cryph::AffVector::cross( b - a, c - b );
  normal.normalize();

  normal.vComponents( _normals[Index] ); a.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); b.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); c.vComponents( _points[Index] ); Index++;
} /* end GeneralMV::tri() */

void GeneralMV::quad( 
		     int a,
		     int b,
		     int c,
		     int d,
		     int& Index          //!> The index in _points and _normals at which to start
		      )
{
  //cryph::AffVector u = cryph::AffVector( _vertices[b] ) - cryph::AffVector( _vertices[a] );
  cryph::AffVector u = cryph::AffVector( _vertices[a] ) - cryph::AffVector( _vertices[b] );
  cryph::AffVector v = cryph::AffVector( _vertices[c] ) - cryph::AffVector( _vertices[b] );

  /* copied (with some modifications) quad() from Interactive Computer Graphics by E. Angel, p. 627 */
  cryph::AffVector normal = cryph::AffVector::cross( u, v );
  normal.normalize();
  
#ifdef __DEBUG__
  vec3 tmp;
  normal.vComponents( tmp );
  cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << endl;
  cout << "normal = (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;
#endif

  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[b], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[c], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[c], sizeof( vec3 ) ); Index++;  
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[d], sizeof( vec3 ) ); Index++;
} /* end GeneralMV::quad() */
