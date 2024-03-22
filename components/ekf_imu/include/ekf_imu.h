#ifndef EKF_IMU_H
#define EKF_IMU_H

#include "mat_utils.h"
#include "array_utils.h"
#include "stdint.h"

#define DEGTORAD (3.1415/180)
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

	matrixf_t R_init;
	matrixf_t Q_init;
	matrixf_t P_init;
	matrixf_t x_init;

} ekf_imu_config_t;

typedef struct
{
	float R_mat[9];
	float Q_mat[49];
	float K_mat[21];
	float P_mat[49];
	float x_mat[7];

	matrixf_t R;
	matrixf_t Q;
	matrixf_t K;
	matrixf_t P;
	matrixf_t x;
} ekf_imu_filter_t;

void ekf_imu_update_state(ekf_imu_filter_t* this, float* acc_measurements);

void ekf_imu_predict_state(ekf_imu_filter_t* this, float* gyro_rates, float dt);

void ekf_imu_get_F(ekf_imu_filter_t* this, matrixf_t* F, float gyro_rates[3], const float dt);

void ekf_imu_get_H(ekf_imu_filter_t* this, matrixf_t* H);

void ekf_imu_get_h(ekf_imu_filter_t* this, matrixf_t* h);

void ekf_imu_set_default_params(ekf_imu_config_t* config, const float R_INITIAL_DIAG, const float Q_INITIAL_DIAG, const float P_INITIAL_DIAG);

void ekf_imu_reset(const ekf_imu_config_t* config, ekf_imu_filter_t* this);

void ekf_imu_init(const ekf_imu_config_t* config, ekf_imu_filter_t* this);

#endif /* ekf_imu_h */
