#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <stdint.h>

void array_utils_mult_scalar_f(const float* in, float* dest, float scalar, uint8_t size);

void array_utils_sub_f(const float* inA, const float* inB, float* dest, uint8_t size);


void array_utils_memcpy(float* in, float* dest, int length);

#endif /* ARRAY_UTILS_H */
