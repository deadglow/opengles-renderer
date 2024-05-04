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
static inline bivector3_t bivector3(float BV1, float BV2, float BV3)      { return (bivector3_t) { BV1, BV2, BV3 }; }

static inline bivector3_t bv3_wedge(const vec3_t u, const vec3_t v) {
	return bivector3(
		u.x * v.y - u.y * v.x,   // XY
		u.y * v.z - u.z * v.y,   // YZ
		u.z * v.x - u.x * v.z    // ZX
	);
}

// -------------------------------------------------
// rotor 3d
//

typedef struct { float s, xy, yz, zx; } rotor3_t;
static inline rotor3_t rotor3(float s, float xy, float xz, float zy) { return (rotor3_t) { s, xy, xz, zy }; }
static inline rotor3_t rot3_identity() { return (rotor3_t) { 1.f, 0.f, 0.f, 0.f }; }

static inline rotor3_t rot3_reverse(const rotor3_t r) {
	return rotor3(r.s, -r.xy, -r.yz, -r.zx);
}

static inline float rot3_lengthsqr(const rotor3_t r) {
	return (r.s * r.s) + (r.xy * r.xy) + (r.yz * r.yz) + (r.zx * r.zx);
}

static inline float rot3_length(const rotor3_t r) {
	return sqrtf(rot3_lengthsqr(r));
}

static inline rotor3_t rot3_normalize(const rotor3_t r) {
	float l = rot3_length(r);
	return rotor3(r.s / l, r.xy / l, r.yz / l, r.zx / l);
}

static inline rotor3_t rot3_mul(const rotor3_t a, const rotor3_t b) {
	return rotor3(
		(a.s * b.s ) - (a.xy * b.xy) - (a.yz * b.yz) - (a.zx * b.zx),
		(a.s * b.xy) + (a.xy * b.s ) - (a.yz * b.zx) + (a.zx * b.yz),
		(a.s * b.yz) + (a.xy * b.zx) + (a.yz * b.s ) - (a.zx * b.xy),
		(a.s * b.zx) - (a.xy * b.yz) + (a.yz * b.xy) + (a.zx * b.s )
	);
}

static inline rotor3_t rot3_plane_angle(const bivector3_t plane, float angleRad) {
	float sina = sinf(angleRad * 0.5f);
	return rotor3(
		cosf(angleRad * 0.5f),
		-sina * plane.xy,
		-sina * plane.xy,
		-sina * plane.zx
	);
}

// r v r*
static inline vec3_t rot3_transform(const rotor3_t r, const vec3_t v)
{
	// q = rv
	vec3_t q = vec3(
		(v.x * r.s) + (v.y * r.xy) - (v.z * r.zx),
		(v.y * r.s) - (v.x * r.xy) + (v.z * r.yz),
		(v.z * r.s) - (v.y * r.yz) + (v.x * r.zx)
	);

	float qxyz = v.x * r.yz + v.y * r.zx + v.z * r.xy;

	// p = q * r^-1
	return vec3(
		(q.x * r.s) + (q.y * r.xy) - (q.z * r.zx) + (qxyz * r.yz),
		(q.y * r.s) - (q.x * r.xy) - (q.z * r.yz) + (qxyz * r.zx),
		(q.z * r.s) + (q.x * r.zx) - (q.y * r.yz) + (qxyz * r.xy)
	);
	// p.xyz = (qxyz * r.s) - (q.x * r.yz) - (q.y * r.zx) - (q.z * r.xy);
	// p.xyz cancels to zero if the calculations for q are substituted in.
}

static inline mat4_t rot3_matrix(const rotor3_t r)
{
	const vec3_t m_x = rot3_transform(r, vec3(1.f, 0.f, 0.f));
	const vec3_t m_y = rot3_transform(r, vec3(0.f, 1.f, 0.f));
	const vec3_t m_z = rot3_transform(r, vec3(0.f, 0.f, 1.f));

	return mat4(
		m_x.x, m_y.x, m_z.x, 0.f,
		m_x.y, m_y.y, m_z.y, 0.f,
		m_x.z, m_y.z, m_z.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}


#endif // MATH_ROTOR_HEADER

#ifdef MATH_ROTOR_IMPLEMENTATION

#endif // MATH_ROTOR_IMPLEMENTATION