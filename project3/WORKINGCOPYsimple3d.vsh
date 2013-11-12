#version 420 core

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.

const int MAX_NUM_LIGHTS = 3;
const int ORTHOGONAL = 1;
const int OBLIQUE = 2;
const int PERSPECTIVE = 3;

uniform int projection_type = 1;

// Transformation Matrices
uniform mat4 M4x4_wc_ec =   // (dynamic rotations) * (ViewOrientation(E,C,up))
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> no dynamic rotations -AND- WC = EC
	     0.0, 0.0, 1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);

uniform mat4 M4x4_ec_lds = // (W-V map) * (projection matrix)
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to (almost) identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> ORTHOGONAL projection -AND- EC = LDS
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
             
uniform vec4 kd = // diffuse reflectivity
	vec4( 0.8, 0.0, 0.0, 1.0 ); // default: dark red

uniform vec4 ka = // ambient color
	vec4( 0.0, 0.0, 0.8, 1.0 ); // default: dark blue

uniform vec4 ks = // incident light
	vec4( 0.0, 0.8, 0.0, 1.0 ); // default: dark green

uniform vec4 p_ecLightPos[MAX_NUM_LIGHTS] =
	{    
	     vec3( 0.0, 0.0, 1.0, 1.0 ),
	     vec3( 0.0, 1.0, 0.0, 1.0 ),
	     vec3( 1.0, 0.0, 0.0, 1.0 )
	};

uniform vec3 lightStrength[MAX_NUM_LIGHTS] =
	{
		vec3( 0.0, 0.0, 1.0 ),
		vec3( 0.0, 0.0, 1.0 ),
		vec3( 0.0, 0.0, 1.0 )
	};

uniform vec4 globalAmbient = 
	vec3( 1.0, 1.0, 1.0 ); // default: white

uniform float m; // specular coefficient


// Per-vertex attributes
// 1. incoming vertex position in world coordinates
layout (location = 0) in vec3 wcPosition;
// 2. incoming vertex normal vector in world coordinates
in vec3 wcNormal; // incoming normal vector in world coordinates

out vec4 colorToFS;

vec4 evaluateLightingModel(in vec3 ec_P, in vec3 ec_nHat)
{
	// Create a unit vector towards the viewer 
	if( projection_type == OBLIQUE )
	{
	}
	else if( projection_type == PERSPECTIVE )
	{
	}
	else // assume ORTHOGONAL
	{
	}

	// if we are viewing this point "from behind", we need to negate
	// the incoming normal vector since our lighting model expressions
	// implicitly assume the normal vector points toward the same
	// side of the triangle that the eye is on.
	

	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )
	{
		// if light is behind this object, skip this light source
		// else:
		//	1. compute the accumlate diffuse contribution
		// 	2. if viewer on appropriate side of normal vector,
		//         compute and accumulate specular contribution.
	}

	return vec4(factor * kd.rgb, kd.a);
}

void main ()
{
	// convert current vertex and its associated normal to eye coordinates
	vec4 p_ecPosition = M4x4_wc_ec * vec4(wcPosition, 1.0);
	mat3 normalMatrix = transpose( inverse( mat3x3(M4x4_wc_ec) ) );
	vec3 ec_nHat = normalize(normalMatrix * wcNormal);

	colorToFS = evaluateLightingModel(p_ecPosition.xyz, ec_nHat);

	// apply the projection matrix to compute the projective space representation
	// of the 3D logical device space coordinates of the input vertex:
	gl_Position =  M4x4_ec_lds * p_ecPosition;
}