precision highp float;

uniform float u_time;

uniform mat4 u_vMat;
uniform mat4 u_ivMat;
uniform mat4 u_pMat;
uniform mat4 u_ipMat;

const int MAX_SPHERES = 10;
const int MAX_STEPS = 200;
const float MAX_DISTANCE = 1000.0;
const vec3 LIGHTDIR = normalize(vec3(1, 1, 0));
const vec3 SKY_COLOUR = vec3(1, 1, 1);

const float THRESHOLD = 0.001;
const float NORMAL_THRESHOLD = 0.0008;

uniform int u_sphereCount;
uniform vec3 u_spherePos[MAX_SPHERES];
uniform float u_sphereRadius[MAX_SPHERES];
uniform vec3 u_sphereColour[MAX_SPHERES];

varying vec3 v_fragPos;

float smoothMax(in float a, in float b, in float k)
{
	return log(exp(k * a) + exp(k * b)) / k;
}

float smoothMin(in float a, in float b, in float k)
{
	return -smoothMax(-a, -b, k);
}

float sdfSphere(in vec3 centre, in float radius, in vec3 point)
{
	return length(centre - point) - radius;
}

float sdfPlane(in float height, in vec3 point)
{
	return point.y - height;
}

float sdfWater(in float height, in float waveHeight, in vec3 point)
{
	float x = sin(point.x + (u_time * 0.01) * 0.0001);
	float y = sin(point.z + (u_time * 0.01) * 0.0001);
	float h = height + (x + y) * 0.5 * waveHeight;
	return sdfPlane(h, point);
}

float getShortestDistance(in vec3 point, out vec3 colour)
{
	float shortest = MAX_DISTANCE;

	colour = vec3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < MAX_SPHERES; i++)
	{
		if (!(i < u_sphereCount)) break;

		float dist = sdfSphere(u_spherePos[i], u_sphereRadius[i], point);

		if (dist > MAX_DISTANCE) break;

		if (dist < shortest)
		{
			shortest = dist;
			colour = u_sphereColour[i];
		}
	}

	float water = sdfWater(0.0, 2.0, point);
	if (water < shortest)
	{
		float waterDist = sdfWater(0.0, .5, point);
		shortest = waterDist;
		colour = vec3(0.6, 0.6, 1);
	}

	return shortest;
}

vec3 getSurfaceNormal(in vec3 p)
{
	vec3 col = vec3(0.0);
    float d0 = getShortestDistance(p, col);
    const vec2 epsilon = vec2(NORMAL_THRESHOLD, 0.0);
    vec3 d1 = vec3(
        getShortestDistance(p - epsilon.xyy, col),
        getShortestDistance(p - epsilon.yxy, col),
        getShortestDistance(p - epsilon.yyx, col)
		);
    return normalize(d0 - d1);
}

vec3 march(in vec3 origin, in vec3 dir, out float travelDistance, out vec3 normal)
{
	vec3 totalLight = vec3(1.0, 1.0, 1.0);
	travelDistance = 0.0;
	normal = dir;

	vec3 point = origin;

	for (int i = 0; i < MAX_STEPS; i++)
	{
		vec3 col;
		float dist = getShortestDistance(point, col);
		travelDistance += dist;
		point += dir * dist;

		if (dist < THRESHOLD)
		{
			totalLight *= col;
			normal = getSurfaceNormal(point);
			dir = reflect(dir, normal);
			point += dir * THRESHOLD * 1.001;
		}

		if (travelDistance > MAX_DISTANCE) break;
	}

	return totalLight;
}

vec3 calcLighting(in vec3 colour, in vec3 normal)
{
	float lVal = clamp(dot(normal, LIGHTDIR), 0.25, 1.0);
	return mix(vec3(0.0, 0.0, 0.0), colour, lVal);
}

void main()
{
	mat4 ivpMat = u_ivMat * u_ipMat; //point
	vec4 origin = vec4(u_ivMat * vec4(0.0, 0.0, 0.0, 1.0));

	// get the furthest point in the frustrum into world space
	vec4 maxPoint = vec4(v_fragPos.x, v_fragPos.y, 1, 1);
	maxPoint = vec4(ivpMat * maxPoint);

	vec3 dir = normalize(vec3(maxPoint) - vec3(origin));

	// we do marching from here
	float travelDistance;
	vec3 normal = vec3(0,0,0);
	vec3 totalLight = march(vec3(origin), dir, travelDistance, normal);

	vec3 colour = SKY_COLOUR * totalLight;
	colour = calcLighting(colour, normal);

	vec3 visualised = (1. + normal) / 2.0;

	gl_FragColor = vec4(visualised, 1.0);
	// gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
} 