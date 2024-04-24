precision highp float;

// vertex buffer
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec4 a_tangent;
attribute vec2 a_texCoord;

// globals
uniform float u_time;
uniform float u_deltaTime;
uniform float u_aspect;

// camera matrices
uniform mat4 u_vMat;
uniform mat4 u_ivMat;
uniform mat4 u_pMat;
uniform mat4 u_ipMat;

// how to do mesh??
uniform mat4 u_m2w;
uniform mat4 u_im2w;

// out
varying vec3 v_fragPos;
varying mat3 v_TBN;
varying vec2 v_texCoord;

void main()
{
	mat4 vm2w = u_vMat * u_m2w;
	vec4 pos4 = vec4(a_position, 1.0);
	v_fragPos = vec3(vm2w * pos4);

	gl_Position = u_pMat * vec4(v_fragPos, 1.0);

	vec3 normal = a_normal;
	vec3 tangent = a_tangent.xyz;
	vec3 biTangent = cross(normal, tangent) * a_tangent.w;

	mat3 normalMat = mat3(vm2w);
	v_TBN = normalMat * mat3(tangent, biTangent, normal);

	v_texCoord = a_texCoord;
}