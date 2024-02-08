precision mediump float;

attribute vec4 a_position;

uniform mat4 u_vMat;
uniform mat4 u_pMat;

varying vec3 v_fragPos;

void main()
{
	v_fragPos = a_position.xyz;
	gl_Position = vec4(a_position.xyz, 1.0);
	// vec4 pos = u_pMat * u_vMat * vec4(a_position.xyz, 1.0);
	// gl_Position = pos;
}