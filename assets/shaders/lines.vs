precision highp float;

// vertex buffer
attribute vec3 a_position;
attribute vec4 a_colour;
attribute float a_blend;

// globals
uniform float u_time;
uniform float u_deltaTime;
uniform float u_aspect;

// camera matrices
uniform mat4 u_vMat;
uniform mat4 u_ivMat;
uniform mat4 u_pMat;
uniform mat4 u_ipMat;

// out
varying vec3 v_fragPos;
varying vec4 v_colour;
varying float v_blend;

void main()
{
	vec4 pos4 = vec4(a_position, 1.0);
	v_fragPos = vec3(u_vMat * pos4);
	
	v_colour = a_colour;
	v_blend = a_blend;

	gl_Position = u_pMat * vec4(v_fragPos, 1.0);
}