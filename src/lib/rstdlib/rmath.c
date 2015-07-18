#define SO_NAME "rstdlib.so"

#include <virtual-machine.h>

extern r3x_cpu_t* CPU; 

typedef union {
	uint32_t Float32;
	float FloatValue;
} r_FloatUnion;

r_FloatUnion ConvUnion;

static inline uint32_t __internal_ConvertFloattoUint32(float Fl32) {
	ConvUnion.Float32 = 0;
	ConvUnion.FloatValue = 0;
	ConvUnion.FloatValue = Fl32;
	return ConvUnion.Float32;
}
static inline float __internal_ConvertUint32toFloat(uint32_t Ui32) {
	ConvUnion.Float32 = 0;
	ConvUnion.FloatValue = 0;
	ConvUnion.Float32 = Ui32;
	return ConvUnion.FloatValue;
}

uint32_t r_sin(void) {
	return __internal_ConvertFloattoUint32(sinf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_cos(void) {
	return __internal_ConvertFloattoUint32(cosf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_tan(void) {
	return __internal_ConvertFloattoUint32(tanf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_cosec(void) {
	return __internal_ConvertFloattoUint32(1/sinf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_sec(void) {
	return __internal_ConvertFloattoUint32(1/cosf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_cot(void) {
	return __internal_ConvertFloattoUint32(1/tanf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_sinh(void) {
	return __internal_ConvertFloattoUint32(sinhf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_cosh(void) {
	return __internal_ConvertFloattoUint32(coshf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_tanh(void) {
	return __internal_ConvertFloattoUint32(tanhf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_cosech(void) {
	return __internal_ConvertFloattoUint32(1/sinhf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_sech(void) {
	return __internal_ConvertFloattoUint32(1/coshf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_coth(void) {
	return __internal_ConvertFloattoUint32(1/tanhf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_asin(void) {
	return __internal_ConvertFloattoUint32(asinf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_acos(void) {
	return __internal_ConvertFloattoUint32(acosf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_atan(void) {
	return __internal_ConvertFloattoUint32(atanf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_acosec(void) {
	return __internal_ConvertFloattoUint32(asinf(1/__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_asec(void) {
	return __internal_ConvertFloattoUint32(acosf(1/__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_acot(void) {
	return __internal_ConvertFloattoUint32(atanf(1/__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_asinh(void) {
	return __internal_ConvertFloattoUint32(asinhf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_acosh(void) {
	return __internal_ConvertFloattoUint32(acoshf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_atanh(void) {
	return __internal_ConvertFloattoUint32(atanhf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_asech(void) {
	return __internal_ConvertFloattoUint32(acoshf(1/__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_acosech(void) {
	return __internal_ConvertFloattoUint32(asinhf(1/__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_acoth(void) {
	return __internal_ConvertFloattoUint32(atanhf(1/__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_exp(void) {
	return __internal_ConvertFloattoUint32(expf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_log(void) {
	return __internal_ConvertFloattoUint32(logf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_log10(void) {
	return __internal_ConvertFloattoUint32(log10f(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_pow(void) {
	return __internal_ConvertFloattoUint32(powf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,2)), __internal_ConvertUint32toFloat(GetArgument(CPU, 2,2))));
}
uint32_t r_sqrt(void) {
	return __internal_ConvertFloattoUint32(sqrtf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_fabs(void) {
	return __internal_ConvertFloattoUint32(fabsf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_floor(void) {
	return __internal_ConvertFloattoUint32(floorf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_ceil(void) {
	return __internal_ConvertFloattoUint32(ceilf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,1))));
}
uint32_t r_fmod(void) {
	return __internal_ConvertFloattoUint32(fmodf(__internal_ConvertUint32toFloat(GetArgument(CPU, 1,2)), __internal_ConvertUint32toFloat(GetArgument(CPU, 2,2))));
}
