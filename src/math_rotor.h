#ifndef MATH_ROTOR_HEADER
#define MATH_ROTOR_HEADER

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_DEG2RAD
#define M_DEG2RAD (M_PI / 180.0)
#endif

#ifndef M_RAD2DEG
#define M_RAD2DEG (180.0 / M_PI)
#endif

typedef struct { float xy, xz, zy; } rotor3_t;
static inline rotor3_t rotor3(float xy, float xz, float zy )        { return (rotor3_t){ xy, xz, zy }; }


#endif // MATH_ROTOR_HEADER

#ifdef MATH_ROTOR_IMPLEMENTATION

#endif // MATH_ROTOR_IMPLEMENTATION