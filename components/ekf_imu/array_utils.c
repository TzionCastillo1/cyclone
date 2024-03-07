#include "array_utils.h"

void array_utils_mult_scalar_f(const float* in, float* dest, float scalar, uint8_t size)
{
	for (int item = 0; item < size; item++) 
		dest[item] = in[item] * scalar;
}

void array_utils_sub_f(const float* inA, const float* inB, float* dest, uint8_t size)
{
	for (int item = 0; item < size; item++)
		dest[item] = inA[item] - inB[item];
}

void array_utils_memcpy(float* in, float* dest, int length)
{
	for (int item = 0; item < length; item++)
		in[item] = dest[item];
}
