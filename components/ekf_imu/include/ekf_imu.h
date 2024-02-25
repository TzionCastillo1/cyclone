#ifndef EKF_IMU_H
#define EKF_IMU_H

typedef struct
{
	//TODO: store the information required to filter. The actual data will not be stored her,
	//but maybe we will store pointers to data buffers.
	
	//NOTE: Matrices are denoted by capital letters, while vectors are lower-case.
	
	//NOTE: I am struggling to setup the data in such a way that the user can provide starting
	//conditions for the filter, but cannot directly access (edit) any of it's variables after
	//that.

	float R_init = NULL;
	float Q_init = NULL;
	float P_init = NULL;
	float x_init = NULL;

} ekf_imu_config_t;

ekf_imu_filter_t* ekf_imu_init(const ekf_imu_config_t* ekf_imu_config);

void ekf_imu_predict(const float* gyro_readings, ekf_imu_filter_t* ekf_imu_filter_t);

void ekf_imu_update(const float* accel_readings, ekf_imu_filter_t* ekf_imu_filter);

void ekf_imu_get_state(float* x_output, ekf_imu_filter_t* ekf_imu_filter);

#endif /* ekf_imu_h */
