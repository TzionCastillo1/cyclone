#ifndef EKF_IMU_H
#define EKF_IMU_H

#include "mat_utils.h"
#include "stdint.h"

typedef struct
{
	//TODO: store the information required to filter. The actual data will not be stored her,
	//but maybe we will store pointers to data buffers.
	
	//NOTE: Matrices are denoted by capital letters, while vectors are lower-case.
	
	//NOTE: I am struggling to setup the data in such a way that the user can provide starting
	//conditions for the filter, but cannot directly access (edit) any of it's variables after
	//that.

	float R[9];
	float Q[49];
	float P[49];
	float x[7];

	matrixf_t R_init;// = {.mat=R, .HEIGHT=3, .HEIGHT=3};
	matrixf_t Q_init;// = {.mat=Q, .HEIGHT=7, .HEIGHT=7};
	matrixf_t P_init;// = {.mat=P, .HEIGHT=7, .HEIGHT=7};
	matrixf_t x_init;// = {.mat=x, .HEIGHT=7, .WIDTH=1};

} ekf_imu_config_t;

typedef struct
{
	float R_mat[9];
	matrixf_t R;
	float Q_mat[49];
	matrixf_t Q;
	float K_mat[21];
	matrixf_t K;
	float P_mat[49];
	matrixf_t P;
	float x_mat[7];
	matrixf_t x;
} ekf_imu_filter_t;

void ekf_imu_get_F(ekf_imu_filter_t* this, matrixf_t F, float gyro_rates[3], const float dt);

#endif /* ekf_imu_h */
