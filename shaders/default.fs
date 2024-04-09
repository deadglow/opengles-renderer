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

// material
uniform vec3 u_albedo;

// in
varying vec3 v_fragPos;
varying mat3 v_TBN;
varying vec2 v_texCoord;

void main()
{
	vec3 light = u_ambientCol;
	
	vec3 normal = v_TBN * vec3(0, 0, 1);
	float lightAmount = dot(normal, u_lightDir);
	light += u_lightCol * lightAmount;

	vec3 litCol = u_albedo * light;

	// gl_FragColor = vec4(litCol, 1.0);
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}