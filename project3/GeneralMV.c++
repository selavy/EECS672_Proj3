// GeneralMV.c++

#include "GeneralMV.h"

using namespace std;

//#define __DEBUG__

#define SQRD( x ) ( x * x )

#define ZEYEMULTIPLIER ( 5.0f )
#define _MULTIPLIER ( 1.8f )
#define MOVESPEED ( 0.1f )
#define MAXZOOM ( -4.0f )
#define MINZOOM ( -1.0f )

/* static */ ShaderIF * GeneralMV::shaderIF = NULL;
/* static */ int GeneralMV::numInstances = 0;
/* static */ GLuint GeneralMV::shaderProgram = 0;

/* static */ GLint GeneralMV::ppuLoc_M4x4_wc_ec = -1;
/* static */ GLint GeneralMV::ppuLoc_M4x4_ec_lds = -1;

// TODO: (high priority) ** DONE **
// remove ppuLoc_lightModel

/* static */ GLint GeneralMV::pvaLoc_wcPosition = -1;
/* static */ GLint GeneralMV::pvaLoc_wcNormal = -1;

// TODO: (high priority)  ** DONE **
// add phong light model location vars
/********************* PHONG LIGHTING MODEL LOC VARS *********/
/* static */ GLint GeneralMV::ppuLoc_lightPosition = -1;
/* static */ GLint GeneralMV::ppuLoc_lightStrength = -1;
/* static */ GLint GeneralMV::ppuLoc_actualNumLights = -1;
/* static */ GLint GeneralMV::ppuLoc_globalAmbient = -1;
/* static */ GLint GeneralMV::ppuLoc_ka = -1;
/* static */ GLint GeneralMV::ppuLoc_kd = -1;
/* static */ GLint GeneralMV::ppuLoc_ks = -1;
/* static */ GLint GeneralMV::ppuLoc_m = -1;
/*********************     END PHONG LOC VARS    *************/

/* static */ vec3 GeneralMV::_eye;                           //!> location of eye
/* static */ vec3 GeneralMV::_up = { 0.0f, 1.0f, 0.0f };     //!> always up will be the y-axis
/* static */ vec3 GeneralMV::_center = { 0.0f, 0.0f, 0.0f }; //!> for now just the origin
/* static */ float GeneralMV::_zpp;                          //!> location of projection plane for perspective projection

// e = normalize( < 3, 1, 5 > )
/* static */ cryph::AffVector GeneralMV::_E = cryph::AffVector( 0.867535f, 0.497376f, 0.00139473 );
/* static */ float GeneralMV::_EX = 0.507093f;
/* static */ float GeneralMV::_EY = 0.169030f;
/* static */ float GeneralMV::_EZ = 0.845154f;
/* static */ float GeneralMV::_F = 3.0f;
/* static */ float GeneralMV::_D = 0.0f;
/* static */ float GeneralMV::_frustum = -3.0f;

// TODO: (high priority) ** DONE **
// Add phong light model variables 
// TODO: (med priority)
// select good locations and strengths
/*********************** PHONG LIGHTING MODEL VARIABLES *******/
/* static */ vec4 GeneralMV::_lightPosition[numLights] = {
  { -1.0, 0.0, 0.5, 0.0 }, // source 0: directional
  {  0.0, 1.0, 1.0, 0.0 }, // source 1: directional
  {  0.0, 0.0, 2.0, 1.0 }  // source 2: positional
};
/* static */ float GeneralMV::_lightStrength[3 * numLights] = {
  1.0, 1.0, 1.0, // source 0: 100% strength white
  0.4, 0.4, 0.4, // source 1:  40% strength white
  0.8, 0.8, 0.0  // source 2:  80% strength yellow
};
/* static */ vec4 GeneralMV::_ambientStrength = { 0.25, 0.25, 0.25, 1.0 }; // assumed ambient light
/********************** END PHONG VARS ************************/

// TODO: (high priority) ** DONE **
// remove _lightloc

/* static */ vec3 GeneralMV::_ecmin = { -1.0, -1.0f, -1.0f };
/* static */ vec3 GeneralMV::_ecmax = {  1.0f, 1.0f,  1.0f };

// TODO: (med priority) ** DONE **
// Change this to perspective
/* static */ GeneralMV::PROJECTION_TYPE GeneralMV::_proj_type = PERSPECTIVE;
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
  // since this is the base class the destructor of the derived class
  // will be called first. so it is the responsibility of the
  // derived class to delete its vao and vbo variables
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
// TODO: (med priority) ** MOVED **
// must calculate a bounding sphere and update the projection matrix
// when the screen is moved.

// TODO: (low priority) (next project)
// standardize key movements
// w,a,s,d ==> up, left, down, right
// z, x ==> zoom in, zoom out
// w,a,s,d should move spherically (i.e. as if defined in spherical coordinates,
// should rotate around the image, not just pan horizontally, vertically

// implement by keeping dynamic rotations matrix

// TODO: (low priority) ** DONE **
// implement min and max for zoom


  if( key == 'q' )
    {
      // TODO: (high priority) ** DONE **
      // switch back
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
      cryph::AffVector moveUp( 0.0f, MOVESPEED, 0.0f );
      _E += moveUp;
    }
  else if( key == 's' )
    {
      cryph::AffVector moveDown( 0.0f, MOVESPEED, 0.0f );
      _E -= moveDown;
    }
  else if( key == 'a' )
    {
      cryph::AffVector moveLeft( MOVESPEED, 0.0f, 0.0f );
      _E -= moveLeft;
    }
  else if( key == 'd' )
    {
      cryph::AffVector moveRight( MOVESPEED, 0.0f, 0.0f );
      _E += moveRight;
    }
  else if( key == 'z' )
    {
      // TODO: (med priority)
      // zoom by moving zpp in _perspective

      // this is not correct.  zoom should be implemented by changing the width and height of frustum
      // will correct in next project
      if( _frustum > MAXZOOM )
	_frustum -= MOVESPEED;
      else
	_frustum = MAXZOOM;
    }
  else if( key == ' ' )
    {
      if( _frustum < MINZOOM )
	_frustum += MOVESPEED;
      else
	_frustum = MINZOOM;
    }

  printEyeLoc();
  printBox();
} /* end GeneralMV::handleCommand() */

// TODO: (high priority) ** DONE **
// Calculate a bounding sphere to determine bounding box for projection functions
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

  _zpp = _frustum * _r;
  _D = _F * _r;

  float tmpE[3];

  _E.normalize();
  _E.vComponents( tmpE );
  _EX = tmpE[0]; _EY = tmpE[1]; _EZ = tmpE[2];
  
#ifdef __DEBUG__
  cout << "E = (" << _EX << ", " << _EY << ", " << _EZ << ")" << endl;
#endif

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

  // TODO: (high priority) ** DONE **
  // adjust limits to keep VAR the same
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

  // TODO: (high priority) ** DONE **
  // Generate _ecmin, _ecmax based upon bounding sphere
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

  // TODO: (high priority) ** DONE **
  // Change the projection matrix based upon _proj_type
  if( _proj_type == ORTHOGONAL )
    {
      //Get the orthogonal projection matrix
      orthogonal(
		 _ecmin[0], _ecmax[0], _ecmin[1],
		 _ecmax[1], _ecmin[2], _ecmax[2],
		 _projection
		 );
  
    }
  else if( _proj_type == OBLIQUE )
    {
      // just using constant for D since I don't really care about oblique projection
      float dx = 0.0f;
      float dy = 0.0995037;
      float dz = 0.995037;
      oblique( _zpp, _ecmin[0], _ecmax[0], _ecmin[1], _ecmax[1], _ecmin[2], _ecmax[2], dx, dy, dz, _projection );
    }
  else //( _proj_type == PERSPECTIVE )
    {
      perspective( _zpp, _ecmin[0], _ecmax[0], _ecmin[1], _ecmax[1], _ecmin[2], _ecmax[2], _projection ); // length(m) == 16
    }
  

  
  glUniformMatrix4fv( GeneralMV::ppuLoc_M4x4_wc_ec, 1, GL_FALSE, _model_view );
  glUniformMatrix4fv( GeneralMV::ppuLoc_M4x4_ec_lds, 1, GL_FALSE, _projection );
} /* end GeneralMV::getMatrices() */

// TODO: (high priority)
// create sendPhongLightModel()
/**
 * send the phong lighting model to the GPU
 * ASSUMES the MV matrix has been updated before this function was called
 */
void GeneralMV::sendPhongLightModel( const vec4& ka, const vec4& kd, const vec4& ks, const float m )
{
  float lightPositionInEC[4 * numLights];

  // convert the light sources to Eye Coordinates
  for( short i = 0; i < numLights; ++i )
    {
      if( _lightPosition[i][4] == 0.0f )
	{
	  // already in eye coordinates
	  lightPositionInEC[i] = _lightPosition[i][0];
	  lightPositionInEC[i+1] = _lightPosition[i][1];
	  lightPositionInEC[i+2] = _lightPosition[i][2];
	  lightPositionInEC[i+3] = _lightPosition[i][3];
	  continue;
	}
      
      cryph::AffVector tmpLightPosition( _lightPosition[i] );
      cryph::Matrix4x4 wcToECMat( _model_view );

      cryph::AffVector ecLightPos = wcToECMat * tmpLightPosition;
      ecLightPos.vComponents( lightPositionInEC, 3 * i );
    }

  // TODO: (high priority) ** DONE **
  // send light positions in EC
  glUniform4fv( ppuLoc_lightPosition, numLights, lightPositionInEC );
  glUniform3fv( ppuLoc_lightStrength, numLights, _lightStrength );
  glUniform1i( ppuLoc_actualNumLights, numLights );
  glUniform4fv( ppuLoc_globalAmbient, 1, _ambientStrength );

  glUniform4fv( ppuLoc_ka, 1, ka );
  glUniform4fv( ppuLoc_kd, 1, kd );
  glUniform4fv( ppuLoc_ks, 1, ks );
  glUniform1f( ppuLoc_m, m );
} /* end GeneralMV::sendPhongLightModel() */

// TODO: (high priority) ** DONE **
// remove sendLightSource

void GeneralMV::fetchGLSLVariableLocations()
{
  if( GeneralMV::shaderProgram > 0 )
    {
      ppuLoc_M4x4_wc_ec = ppUniformLocation( shaderProgram, "M4x4_wc_ec" );
      ppuLoc_M4x4_ec_lds = ppUniformLocation( shaderProgram, "M4x4_ec_lds" );

      // TODO: (high priority) ** DONE **
      // get rid of this light source (switching to phong light model)

      pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );

      // TODO: (high priority) ** DONE **
      // get locations of phong lighting model variables
      /***************** PHONG LIGHTING MODEL VARIABLES *********/
      ppuLoc_lightPosition = ppUniformLocation( shaderProgram, "p_ecLightPos" );
      ppuLoc_lightStrength = ppUniformLocation( shaderProgram, "lightStrength" );
      ppuLoc_actualNumLights = ppUniformLocation( shaderProgram, "actualNumLights" );
      ppuLoc_globalAmbient = ppUniformLocation( shaderProgram, "globalAmbient" );
      ppuLoc_ka = ppUniformLocation( shaderProgram, "ka" );
      ppuLoc_kd = ppUniformLocation( shaderProgram, "kd" );
      ppuLoc_ks = ppUniformLocation( shaderProgram, "ks" );
      ppuLoc_m  = ppUniformLocation( shaderProgram, "m"  );
      /*****************       END PHONG VARS           *********/
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
  cryph::AffVector u = cryph::AffVector( _vertices[a] ) - cryph::AffVector( _vertices[b] );
  cryph::AffVector v = cryph::AffVector( _vertices[c] ) - cryph::AffVector( _vertices[b] );

  /* copied (with some modifications) quad() from Interactive Computer Graphics by E. Angel, p. 627 */
  cryph::AffVector normal = cryph::AffVector::cross( u, v );
  normal.normalize();
  
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[b], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[c], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[c], sizeof( vec3 ) ); Index++;  
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[d], sizeof( vec3 ) ); Index++;
} /* end GeneralMV::quad() */
