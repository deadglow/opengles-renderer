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
	vec3 albedo = vec3(0.9, 0.0, 0.3);

	vec3 normal = v_TBN * vec3(0, 0, 1);
	vec3 lightDir = mat3(u_vMat) * u_lightDir;
	float lightAmount = dot(normal, normalize(lightDir));
	
	vec3 light = u_ambientCol + (u_lightCol * lightAmount);

	vec3 litCol = albedo * light;

	gl_FragColor = vec4(litCol, 1.0);
	// gl_FragColor = vec4(albedo, 1.0);
}