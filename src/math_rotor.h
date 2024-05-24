#ifndef MATH_ROTOR_HEADER
#define MATH_ROTOR_HEADER

#include "math_3d.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_DEG2RAD
#define M_DEG2RAD (M_PI / 180.0)
#endif

#ifndef M_RAD2DEG
#define M_RAD2DEG (180.0 / M_PI)
#endif

// basis vectors: x, y, z
// basis bivectors: xy, yz, zx

// -------------------------------------------------
// bivectors
//

typedef struct { float xy, yz, zx; } bivector3_t;
static inline bivector3_t bivector3(float xy, float yz, float zx) { return (bivector3_t) { xy, yz, zx }; }

static inline bivector3_t bv3_xy{ 1.f, 0.f, 0.f };
static inline bivector3_t bv3_yz{ 0.f, 1.f, 0.f };
static inline bivector3_t bv3_zx{ 0.f, 0.f, 1.f };

static inline bivector3_t bv3_wedge(const vec3_t u, const vec3_t v) {
	return bivector3(
		u.x * v.y - u.y * v.x,   // XY
		u.y * v.z - u.z * v.y,   // YZ
		u.z * v.x - u.x * v.z    // ZX
	);
}

// -------------------------------------------------
// supplementary quat
//
typedef struct { float i, j, k, s; } quat_t;
static inline quat_t quat(float i, float j, float k, float s) { return (quat_t) { i, j, k, s }; }
static inline quat_t quat_identity() { return (quat_t) { 1.f, 0.f, 0.f, 0.f }; }

// -------------------------------------------------
// rotor 3d
//

typedef struct { float s, xy, yz, zx; } rotor3_t;
static inline rotor3_t rotor3(float s, float xy, float xz, float zy) { return (rotor3_t) { s, xy, xz, zy }; }
static inline rotor3_t rot3_identity() { return (rotor3_t) { 1.f, 0.f, 0.f, 0.f }; }

static inline quat_t rot3_toquat(rotor3_t r) { return (quat_t) { -r.yz, -r.zx, -r.xy, r.s }; }
static inline rotor3_t rot3_fromquat(quat_t q) { return (rotor3_t) { q.s, -q.k, -q.i, -q.j }; }

static inline rotor3_t rot3_reverse(const rotor3_t r) {
	return rotor3(r.s, -r.xy, -r.yz, -r.zx);
}

static inline float rot3_lengthsqr(const rotor3_t r) {
	return (r.s * r.s) + (r.xy * r.xy) + (r.yz * r.yz) + (r.zx * r.zx);
}

static inline float rot3_length(const rotor3_t r) {
	return sqrtf(rot3_lengthsqr(r));
}

// double verified on paper
static inline rotor3_t rot3_mul(const rotor3_t a, const rotor3_t b)
{
	return rotor3(
		(a.s * b.s) - (a.xy * b.xy) - (a.yz * b.yz) - (a.zx * b.zx),
		(a.s * b.xy) + (a.xy * b.s ) - (a.yz * b.zx) + (a.zx * b.yz),
		(a.s * b.yz) + (a.xy * b.zx) + (a.yz * b.s ) - (a.zx * b.xy),
		(a.s * b.zx) - (a.xy * b.yz) + (a.yz * b.xy) + (a.zx * b.s )
	);
};

rotor3_t rot3_normalize(const rotor3_t r);
rotor3_t rot3_plane_angle(const bivector3_t plane, float angleRad);
rotor3_t rot3_from_to(const vec3_t from, vec3_t to);
rotor3_t rot3_from_to_fast(const vec3_t from, vec3_t to);
rotor3_t rot3_nlerp(const rotor3_t a, const rotor3_t b, float t);
rotor3_t rot3_look_rotation(const vec3_t forward, const vec3_t up);
rotor3_t rot3_look_rotation_nochecks(const vec3_t forward, const vec3_t up);
vec3_t rot3_transform(const rotor3_t r, const vec3_t v);
mat4_t rot3_matrix(const rotor3_t r);
mat4_t m4_trs(const vec3_t t, const rotor3_t r, const float s);


#endif // MATH_ROTOR_HEADER

#ifdef MATH_ROTOR_IMPLEMENTATION

rotor3_t rot3_normalize(const rotor3_t r)
{
	float l = rot3_length(r);
	return rotor3(r.s / l, r.xy / l, r.yz / l, r.zx / l);
}

rotor3_t rot3_plane_angle(const bivector3_t plane, float angleRad)
{
	angleRad = angleRad * 0.5f;
	float sina = sinf(angleRad);
	return rotor3(
		cosf(angleRad),
		sina * plane.xy,
		sina * plane.yz,
		sina * plane.zx
	);
}

// ensure from and to are normalized
// #NEEDS-CHECKING
rotor3_t rot3_from_to(const vec3_t from, vec3_t to)
{
	const float theta = acosf(v3_dot(from, to));
	const float cosHalfTheta = cosf(theta * 0.5f);
	const float sinHalfTheta = sinf(theta * 0.5f);

	const bivector3_t wedge = bv3_wedge(to, from);
	return rotor3(
		cosHalfTheta,
		sinHalfTheta * wedge.xy,
		sinHalfTheta * wedge.yz,
		sinHalfTheta * wedge.zx
	);
}

// ensure from and to are normalized
// ensure from and to are not facing directly away from each other
// #NEEDS-CHECKING
rotor3_t rot3_from_to_fast(const vec3_t from, vec3_t to)
{
	const vec3_t halfway = v3_norm(v3_add(from, to));

	const bivector3_t wedge = bv3_wedge(halfway, from);
	float fDotH = v3_dot(from, halfway);
	return rotor3(
		fDotH,
		wedge.xy,
		wedge.yz,
		wedge.zx
	);
}

rotor3_t rot3_nlerp(const rotor3_t a, const rotor3_t b, float t)
{
	const float dot = a.s * b.s + a.xy * b.xy + a.yz * b.yz + a.zx * b.zx;
	rotor3_t signedA = a;
	if (dot < 0.f)
	{
		signedA.s = -a.s;
		signedA.xy = -a.xy;
		signedA.yz = -a.yz;
		signedA.zx = -a.zx;
	}

	rotor3_t r = {
		lerp(signedA.s, b.s, t),
		lerp(signedA.xy, b.xy, t),
		lerp(signedA.yz, b.yz, t),
		lerp(signedA.zx, b.zx, t),
	};

	return rot3_normalize(r);
}

// #NEEDS-CHECKING
rotor3_t rot3_look_rotation(const vec3_t forward, const vec3_t up)
{
	if (v3_dot(forward, forward) < 0.00001f)
	{
		return rot3_identity();
	}
	
	float fDotU = v3_dot(forward, up);
	if (fDotU > 0.999f)
	{
		vec3_t v = v3_muls(v3_add(forward, up), -fDotU);
		rotor3_t r1 = rot3_from_to(v3_forward, v);
		rotor3_t r2 = rot3_from_to(v, forward);
		return rot3_mul(r1, r2);
	}
	else
	{
		return rot3_from_to(v3_forward, forward);
	}
}

// ensure forward and up are normalized unit vectors and aren't the same
// #NEEDS-CHECKING
rotor3_t rot3_look_rotation_nochecks(const vec3_t forward, const vec3_t up)
{
	float fDotU = v3_dot(forward, up);
	vec3_t v = v3_muls(v3_add(forward, up), -fDotU);
	rotor3_t r1 = rot3_from_to(v3_forward, v);
	rotor3_t r2 = rot3_from_to(v, forward);
	return rot3_mul(r1, r2);
}

// checked twice
// r v r*
vec3_t rot3_transform(const rotor3_t r, const vec3_t v)
{
	// s = rv
	vec3_t s = vec3(
		(v.x * r.s) + (v.y * r.xy) - (v.z * r.zx),
		(v.y * r.s) - (v.x * r.xy) + (v.z * r.yz),
		(v.z * r.s) - (v.y * r.yz) + (v.x * r.zx)
	);

	float sxyz = v.x * r.yz + v.y * r.zx + v.z * r.xy;

	rotor3_t g = rot3_reverse(r);

	// p = q * r^-1
	vec3_t v3 = vec3(
		(s.x * g.s) - (s.y * g.xy) + (s.z * g.zx) - (sxyz * g.yz),
		(s.y * g.s) + (s.x * g.xy) - (s.z * g.yz) - (sxyz * g.zx),
		(s.z * g.s) - (s.x * g.zx) + (s.y * g.yz) - (sxyz * g.xy)
	);
	return v3;

	// p.xyz = (sxyz * r.s) - (s.x * r.yz) - (s.y * r.zx) - (s.z * r.xy);
	// p.xyz cancels to zero if the calculations for s are substituted in.
}

// #NEEDS-CHECKING
mat4_t rot3_matrix(const rotor3_t r)
{
	const vec3_t m_x = rot3_transform(r, vec3(1.f, 0.f, 0.f));
	const vec3_t m_y = rot3_transform(r, vec3(0.f, 1.f, 0.f));
	const vec3_t m_z = rot3_transform(r, vec3(0.f, 0.f, 1.f));

	return mat4(
		m_x.x, m_y.x, m_z.x, 0.f,
		m_x.y, m_y.y, m_z.y, 0.f,
		m_x.z, m_y.z, m_z.z, 0.f,
		0.f,   0.f,   0.f,   1.f
	);
}

// #NEEDS-CHECKING
mat4_t m4_trs(const vec3_t t, const rotor3_t r, const float s)
{
	const vec3_t m_x = rot3_transform(r, vec3(s, 0.f, 0.f));
	const vec3_t m_y = rot3_transform(r, vec3(0.f, s, 0.f));
	const vec3_t m_z = rot3_transform(r, vec3(0.f, 0.f, s));

	return mat4(
		m_x.x, m_y.x, m_z.x, t.x,
		m_x.y, m_y.y, m_z.y, t.y,
		m_x.z, m_y.z, m_z.z, t.z,
		0.f,   0.f,   0.f,   1.f
	);
}

#endif // MATH_ROTOR_IMPLEMENTATION