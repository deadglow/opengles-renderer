precision highp float;

uniform float u_time;

uniform mat4 u_vMat;
uniform mat4 u_ivMat;
uniform mat4 u_pMat;
uniform mat4 u_ipMat;

const int MAX_STEPS = 200;
const float MAX_DISTANCE = 150.0;
const vec3 LIGHTDIR = normalize(vec3(1, 1, 0));
const vec3 SKY_COLOUR = vec3(0.4, 0.9, 0.4);
const vec3 WATER_COLOUR = vec3(0.2, 0.5, 0.8);

const float THRESHOLD = 0.1;
const float NORMAL_THRESHOLD = 0.1;

varying vec3 v_fragPos;

float round(in float v)
{
	return floor(v + 0.5);
}

bool approximately(in float a, in float b)
{
	return abs(a - b) < 0.000001;
}

float smoothMax(in float a, in float b, in float k)
{
	return log(exp(k * a) + exp(k * b)) / k;
}

float smoothMin(in float a, in float b, in float k)
{
	return -smoothMax(-a, -b, k);
}

// this bias seems to scale the distance for each march.
// it will slow things down, but ensures that the modulation of the water won't have artifacts.
const float slopeUpperBound = 2.;
const float slopeBias = sin(atan(1., slopeUpperBound));
float sampleDistToWater(in vec3 point)
{
	return point.y;
	float h = sin(point.x) + sin(point.z);
	return (point.y - h) * slopeBias;
}

float getWorldDistance(in vec3 point)
{
	// get water height
	float shortest;

	shortest = sampleDistToWater(point);

	// compare to other shapes (maybe with smoothmin)

	return shortest;
}

vec3 computeNormal(in vec3 p)
{
	float d0 = getWorldDistance(p);
	const vec2 epsilon = vec2(NORMAL_THRESHOLD, 0.0);
	vec3 d1 = vec3(
		getWorldDistance(p - epsilon.xyy),
		getWorldDistance(p - epsilon.yxy),
		getWorldDistance(p - epsilon.yyx));

	return normalize(d0 - d1);
}

bool march(in vec3 point, in vec3 dir, out float shortestDistance, out float travelDistance, out vec3 hitPoint)
{
	travelDistance = 0.;
	shortestDistance = MAX_DISTANCE;

	for (int i = 0; i < MAX_STEPS; i++)
	{
		float worldDist = getWorldDistance(point);
		shortestDistance = min(shortestDistance, worldDist);

		if (worldDist < THRESHOLD)
		{
			hitPoint = point;
			return true;
		}

		travelDistance += worldDist;
		if (travelDistance > MAX_DISTANCE) break;
		
		point += dir * worldDist;
	}

	return false;
}

float computeLighting(in vec3 normal)
{
	float d = clamp(dot(normal, LIGHTDIR), 0., 1.);
	return mix(0.01, 1.0, d);
}

void computeFragOrigin(out vec3 origin, out vec3 dir)
{
	mat4 ivpMat = u_ivMat * u_ipMat; //point
	vec4 origin = vec4(u_ivMat * vec4(0.0, 0.0, 0.0, 1.0));

	// get the furthest point in the frustrum into world space
	vec4 maxPoint = vec4(v_fragPos.x, v_fragPos.y, 1, 1);
	maxPoint = vec4(ivpMat * maxPoint);

	dir = normalize(vec3(maxPoint) - vec3(origin));
}

void main()
{
	vec3 origin, dir;
	computeFragOrigin(origin, dir);

	// we do marching from here
	float travelDistance, shortestDistance;
	vec3 hitPoint;
	bool hit = march(vec3(origin), dir, shortestDistance, travelDistance, hitPoint);

	vec3 colour = SKY_COLOUR;
	if (hit)
	{
		// compute normal and do lighting here
		vec3 normal = computeNormal(hitPoint);
		float light = computeLighting(normal);
		colour = mix(vec3(0.1), WATER_COLOUR, (travelDistance / MAX_DISTANCE) * 3.);
	}

	gl_FragColor = vec4(colour, 1.0);
}
