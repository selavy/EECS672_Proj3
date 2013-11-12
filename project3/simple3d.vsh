#version 420 core

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.

// Naming convention for variables holding coordinates:
// wc - world coordinates
// ec - eye coordinates
// lds - logical device space
// "p_" prefix on any of the preceding indicates the coordinates have been
//      embedded in projective space
// (gl_Position would have label p_lds)

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
             
// There are MANY ways to deal with the basic object color.
// For now we will  simply assume:
uniform vec4 kd = // "kd" - diffuse reflectivity; basic object color
	vec4(0.8, 0.0, 0.0, 1.0); // default: darkish red

uniform vec3 lightModel =
	vec3( 0.0, 0.0, 1.0 );

// Per-vertex attributes
// 1. incoming vertex position in world coordinates
layout (location = 0) in vec3 wcPosition;
// 2. incoming vertex normal vector in world coordinates
in vec3 wcNormal; // incoming normal vector in world coordinates

out vec4 colorToFS;

vec4 evaluateLightingModel(in vec3 ec_P, in vec3 ec_nHat)
{
	// Simplistic lighting model:
	// Assume light is coming from eye towards object. Reflected color
	// will be a function of angle between the normal vector and
	// the light source. Specifically:

	float factor = dot( lightModel, ec_nHat );

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
