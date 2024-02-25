#ifndef MAT_UTILS_H
#define MAT_UTILS_H

/* NOTE: Create an identity matrix of size: A_size */
void mat_utils_eye(float* A, const uint8_t A_size);

void mat_utils_transpose(const float* A, float* A_transpose, const uint8_t A_height, const uint8_t A_width);

void mat_utils_c_mult_scalar(float* A, const float scalar, const uint8_t A_height, const uint8_t A_width);

void mat_utils_mult_scalar(const float* A, const float scalar, float* B, const uint8_t A_height, const uint8_t A_width);

void mat_utils_c_add(float* A, const float* B, const uint8_t height, const uint8_t width);

void mat_utils_c_sub(float* A, const float* B, const uint8_t height, const uint8_t width);

/* NOTE: mat_utils_inverse requires the input array to be square */
void mat_utils_inverse_3x3(const float* A, float* A_inv);

void mat_utils_skew_quat(const float* q, float* S_q);

void mat_utils_quat_norm(float* q);

#endif /* mat_utils_h */
