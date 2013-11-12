// GeneralMV.h

#ifndef GENERAL_MV_H
#define GENERAL_MV_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "AffVector.h"
#include "AffPoint.h"
#include <cmath>
#include "Controller.h"
#include "MatrixIF.h"
#include <cstring>
#include <iostream>

typedef float mat4[16]; // specify in row major, and will always transpose with openGL
typedef float vec3[3];
typedef float vec4[4];

class GeneralMV : public ModelView
{
 public:
  /* types */
  typedef enum PROJECTION_TYPE {
    ORTHOGONAL = 0,
    OBLIQUE,
    PERSPECTIVE
  } PROJECTION_TYPE;

 public:
	GeneralMV();
	virtual ~GeneralMV();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	virtual void getWCBoundingBox(double* xyzLimitsF) const = 0;
	virtual void render() = 0;
	virtual void handleCommand( unsigned char key, double ldsX, double ldsY );

	void setProjectionType( PROJECTION_TYPE proj_type );
	
 protected:
	void calcBoundingSphere();
	void setECMinMax();
	void getMatrices( double limits[6] );
	void sendLightSource( vec4 kd );
	void tri( const vec3& veca, const vec3& vecb, const vec3& vecc, int& Index );
	void quad( int a, int b, int c, int d, int& Index );
	void printBox();
	void printEyeLoc();

	vec3 * _points;
	vec3 * _normals;
	vec3 * _vertices;

	static GLint pvaLoc_wcPosition;
	static GLint pvaLoc_wcNormal;

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint ppuLoc_M4x4_wc_ec;
	static GLint ppuLoc_M4x4_ec_lds;
	static GLint ppuLoc_lightModel;
	static GLint ppuLoc_kd;

	static void fetchGLSLVariableLocations();

	static vec3 _eye;    //!> The location of the camera in World Coordinates
	static vec3 _center; //!> Where the camera is pointed in World Coordinates
	static vec3 _up;        //!> Vector to define which direction is up
	static vec3 _lightloc;  //!> The location of the single light source in the scene
	static vec3 _ecmin;        //!> The model's bounding box mins in eye coordinates
	static vec3 _ecmax;        //!> The model's bounding box maxs in eye coordinates
	static float _r; //!> Bounding sphere radius

	static PROJECTION_TYPE _proj_type;
	mat4 _model_view;   //!> The matrix to convert from eye coordinates to LDS
	mat4 _projection;   //!> The orthogonal projection matrix

};

#endif
