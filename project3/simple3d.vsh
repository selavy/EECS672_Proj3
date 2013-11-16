#version 420 core

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.

const int MAX_NUM_LIGHTS = 3;

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

// PHONG LIGHTING MODEL //

uniform vec4 p_ecLightPos[MAX_NUM_LIGHTS];
uniform vec3 lightStrength[MAX_NUM_LIGHTS];
uniform int actualNumLights = 1;
uniform vec4 globalAmbient = vec4( 1.0, 1.0, 1.0, 1.0 ); // default: white
uniform vec4 ka; // ambient color
uniform vec4 kd; // "kd" - diffuse reflectivity;
uniform vec4 ks; // incident light
uniform float m; // specular coefficient

// END PHONG LIGHTING MODEL //

// TODO: (high priority) ** DONE **
// remove this light source

// Per-vertex attributes
// 1. incoming vertex position in world coordinates
layout (location = 0) in vec3 wcPosition;
// 2. incoming vertex normal vector in world coordinates
in vec3 wcNormal; // incoming normal vector in world coordinates

out vec4 colorToFS;

// TODO: (high priority) ** DONE **
// implement Phong lighting model
vec4 evaluateLightingModel(in vec3 ec_Q, in vec3 ec_nHat)
{
	vec4 I_q = { 0.0, 0.0, 0.0, 0.0 };

	// ambientFactor;
	I_q = ka * globalAmbient;
	I_q.w = ks.w;
 
	vec3 ec_v = { 0.0, 0.0, 1.0 };

	// Create a unit vector towards the viewer (method depends on type of projection)
	// if projection type == orthogonal, v = (0, 0, 1)
	if( M4x4_ec_lds[3][3] != 1.0f )
	    {
		// perspective
		ec_v = -normalize( ec_Q );
		
	    }
	else if( M4x4_ec_lds[1][0] != 0.0f && M4x4_ec_lds[2][0] != 0.0f )
	    {
		// oblique ** DONE **
		// TODO: (high priority)
		// how to calc V
		// not supporting changing D so I know the value a priori
		ec_v = vec3( 0.0, 0.0995037, 0.995037 );
	    }
	else
	    {
		// orthogonal
		// ec_v = { 0.0, 0.0, 1.0 };
	    }

	// if we are viewing this point "from behind", we need to negate the incoming
	// normal vector since our lighting model expressions implicitly assume the normal
	// vector points toward the same side of the triangle that the eye is on.
	if( ec_v.z < ec_Q.z )
	    {
		ec_Q = -1.0f * ec_Q;
	    }

	for( int i = 0; i < actualNumLights; ++i )
	     {
			vec3 tmpacc = { 0.0f, 0.0f, 0.0f };

			// create vector to light source
			vec3 ec_li;
			ec_li.xyz = p_ecLightPos[i].xyz;

			if( p_ecLightPos[i].w != 0.0 )
			{
				ec_li = ec_li - ec_Q;
			}

			ec_li = normalize( ec_li );

			// if light is behind this object, skip this light source 
			float diffuse = dot( ec_nHat, ec_li );
			if( diffuse > 0.0f )
			{
			
				//tmpacc.xyz = kd.xyz * diffuse;
				I_q.xyz += kd.xyz * diffuse * lightStrength[i].xyz;

		        	// 2. if viewer on appropriate side of normal vector,
		       		// compute and accumulate specular contribution
				if( dot( ec_li, ec_nHat ) > 0.0 )
				{
					// compute r_i
					vec3 r_i = ec_nHat;
			 		r_i = r_i * 2;
					r_i = r_i * dot( ec_li, ec_nHat );
					r_i = r_i - ec_li;
				
					float specular = pow( max( dot( r_i, ec_v ), 0.0 ), m );

					I_q.xyz += ks.xyz * specular * lightStrength[i].xyz;
				}
		    	}
	      }
     return I_q;	      
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
