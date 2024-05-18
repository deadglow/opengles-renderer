precision highp float;

// globals
uniform float u_time;
uniform float u_deltaTime;
uniform float u_aspect;

// camera matrices
uniform mat4 u_vMat;
uniform mat4 u_ivMat;
uniform mat4 u_pMat;
uniform mat4 u_ipMat;

// lighting
uniform vec3 u_ambientCol;
uniform vec3 u_lightDir;
uniform vec3 u_lightCol;

// in
varying vec3 v_fragPos;
varying vec4 v_colour;
varying float v_blend;

void main()
{
	gl_FragColor = v_colour;
}