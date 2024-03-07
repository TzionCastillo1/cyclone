#include "ekf_imu.h"
#include <stdio.h>

/* NOTE: The below matrices are square and are therefore only defined by one "size" value */

#define P_SIZE 7
#define F_SIZE 7
#define H_HEIGHT 3
#define H_WIDTH 7


void ekf_imu_init(const ekf_imu_config_t* ekf_imu_config, ekf_imu_filter_t* this)
{
	this->R.HEIGHT = ekf_imu_config->R_init.HEIGHT;
	this->R.WIDTH = ekf_imu_config->R_init.WIDTH;
	mat_utils_copy(config->R_init, this->R);

	this->Q.HEIGHT = ekf_imu_config->Q_init.HEIGHT;
	this->Q.WIDTH = ekf_imu_config->Q_init.WIDTH;
	mat_utils_copy(config->Q_init, this->Q);

	this->P.HEIGHT = ekf_imu_config->P_init.HEIGHT;
	this->P.WIDTH = ekf_imu_config->P_init.WIDTH;
	mat_utils_copy(config->P_init, this->P);

	this->x.HEIGHT = ekf_imu_config->x_init.HEIGHT;
	this->x.WIDTH = ekf_imu_config->x_init.WIDTH;
	mat_utils_copy(config->x_init, this->x);

}

void ekf_imu_set_default_params(ekf_imu_config_t* config, const float R_INITIAL_DIAG, const float Q_INITIAL_DIAG, const float P_INITIAL_DIAG)
{
	mat_utils_eye(config->R_init);
	mat_utils_c_mult_scalar(config->R_init, R_INITIAL_DIAG);

	mat_utils_eye(config->Q_init);
	mat_utils_c_mult_scalar(config->Q_init, Q_INITIAL_DIAG);

	mat_utils_eye(config->P_init);
	mat_utils_eye(config->P_init, P_INITIAL_DIAG);
}

void ekf_imu_reset(const ekf_imu_config_t* config, ekf_imu_filter_t* this)
{
	ekf_imu_init(config, this);
	mat_utils_zeros(this->K);
}

void ekf_imu_get_F(ekf_imu_filter_t* this,  matrixf_t* F, float gyro_rates[3], const float dt)
{
	float half_dt = dt/2;
	float adj_rates[3] = {gyro_rates[0] - this->x[4], 
								gyro_rates[1] - this->x[5], 
								gyro_rates[2] - this->x[6]}; 

	float F_tmp_raw[49] = {1, -half_dt*adj_rates[0], -half_dt*adj_rates[1], -half_dt*adj_rates[2], -(this->x[1]), -(this->x[2]), -(this->x[3],
							half_dt*adj_rates[0], 1, half_dt*adj_rates[2], -half_dt*adj_rates[1], this->x[0], -(this->x[3]), this->x[2],
							half_dt*adj_rates[1], -half_dt*adj_rates[2], 1, half_dt*adj_rates[0], this->x[3], this->x[0], -(this->x[1]),
							half_dt*adj_rates[2], half_dt*adj_rates[1], -half_dt*adj_rates[0], 1, -(this->x[2]), this->x[1], this->x[0],
							0, 0, 0, 0, 1, 0, 0,
							0, 0, 0, 0, 0, 1, 0, 
							0, 0, 0, 0, 0, 0, 1};
	memcpy(F->mat, F_tmp_raw, 49); 	
}


void ekf_imu_predict_state(ekf_imu_filter_t* this)
{
}

void ekf_imu_get_K_gain(ekf_imu_filter_t* this)
{
}

void ekf_imu_filter_update_state(ekf_imu_filter_t* this)
{
}

void ekd_imu_filter_update_P(ekf_imu_filter_t* this)
{
}

/* NOTE: The gyro readings are in units of rad/s */
void ekf_imu_predict(const float* gyro_readings, ekf_imu_filter_t* ekf_imu_filter, const float delta_t)
{

	ekf_imu_predict_P(this, gyro_readings);
	float P_temp[P_SIZE*P_SIZE];

	/* TODO: Provide the state function jacobian, F_x, for this function to use. Options include
	 * passing a function pointer to this funciton which would calculate F_x given x, or building this functionality
	 * in as a part of this function*/

	dspm_mult_f32_aes3(F, P_estimate, P_temp, F_SIZE, F_SIZE, P_SIZE);
	float F_transpose[F_SIZE*F_SIZE];
	mat_utils_transpose(F, F_transpose, F_SIZE, F_SIZE);
	dspm_mult_f32_aes3(P_temp, F_inv, ekf_imu_filter->P_prediction, P_SIZE, P_SIZE, F_SIZE);
	mat_utils_skew_quat(x_estimate, skew_q_curr);
	
	/* NOTE: We are using the kinematic quaternion equation to update the quaternion through angular velocities.
	 * This assumes that the gyro readings and gyro biases are provided to the filter in rad/s. It works through 
	 * small angle approximations of the trigonometric functions that can be used to describe a rotation quaternion */

	float q_gyro_rates[4];
	dspm_mult_f32_aes3(skew_q_curr, gyro_readings, q_gyro_rates, 4,3,1);
	mat_utils_c_mult_scalar(q_gyro_rates, (dt/2.0f), 4, 1);

	/* NOTE: The second parameter of the following function call shifts the pointer ahead by 
	 * 4 spaces in order to access the gyro bias portion of the state*/
	float q_gyro_bias[4];
	dspm_mult_f32_aes3(skew_q_curr, ekf_imu_filter->x_estimate + 4, q_gyro_bias, 4, 3, 1);
	mat_utils_c_mult_scalar(q_gyro_rates, (-dt/2.0f), 4, 1);
	
	x_prediction = x_estimate;

	mat_utils_c_add(x_estimate, q_gyro_rates, 4, 1);
	mat_utils_c_add(x_estimate, q_gyro_bias, 4, 1);

}

void ekf_imu_update(const float* accel_readings, ekf_imu_filter_t* ekf_imu_filter)
{
	/* TODO: This function will implement the code to innovate, or filter, our predicted state.
	 * This will be done through the use of Jacobians for the non-linear state and measurement 
	 * equations, f(x)' = F_x, h(x)' = H_x. These jacobians are used to provide a linear estimate
	 * of the actual functions, centered at the current state. This linearity is a requirement for
	 * the use of a kalman filter, as otherwise we would be transforming the gaussian noise random
	 * variables into some non-gaussian pdf. */
	// NOTE: K = P_prediction*H_transpose*(H*P_prediction*H_transpose + R)^-1

	float H_transpose[H_WIDTH*H_HEIGHT];
	mat_utils_transpose(H, H_transpose, H_HEIGHT, H_WIDTH);
	// NOTE: K_temp_1 = P_prediction * H_tranpose
	float K_temp_1[7*3];
	dspm_mult_f32_aes3(ekf_imu_filter->P_prediction, H_transpose, K_temp_1 P_SIZE, P_SIZE, H_HEIGHT);

	// NOTE: K_temp_2 = H*K_temp_1
	float K_temp_2[7*3];
	dspm_mult_f32_aes3(H, K_temp_1, K_temp_2, H_HEIGHT, H_WIDTH, H_HEIGHT);

	// NOTE: K_temp_2 = K_temp_2 + R
	mat_utils_c_add(K_temp_2, R, H_HEIGHT, H_HEIGHT);

	// NOTE: K_temp_2 = (K_temp_2)^-1
	mat_utils_inverse(K_temp_2, K_temp_2, H_HEIGHT);

	dspm_mult_f32_aes3(K_temp_1, K_temp_2, K, K_HEIGHT, K_WIDTH, K_WIDTH);

	ekf_imu_filter->x_estimate = ekf_imu_filter->x_prediction;

	// NOTE: y_innovation = y - h(x)
	float y_innovation[3*1];
	mat_utils_c_sub(accel_readings, h, 3, 1);

	// NOTE: x_temp = K*(y_innovation - h(x)
	float x_temp[7*1];
	dspm_mult_f32_aes3(K, y_innovation, x_temp, K_HEIGHT, K_WIDTH, 1);

	// NOTE: x_estimate = x_prediction + x_temp
	mat_utils_c_add(x_estimate, x_temp, 7, 1);

	mat_utils_q_normalize(x_estimate);

	// NOTE: P_estimate_temp_1 = K*H
	float P_estimate_temp_1[P_SIZE*P_SIZE];
	dspm_mult_f32_aes3(K, H, P_estimate_temp_1, K_HEIGHT, K_WIDTH, H_WIDTH);
	
	// NOTE: P_estimate_temp_2 = I - P_estimate_temp_1
	float P_estimate_temp_2[P_SIZE*P_SIZE];
	mat_utils_eye(P_estimate_temp_2, P_SIZE);
	mat_utils_c_sub(P_estimate_temp_2, P_estimate_temp_1, P_SIZE, P_SIZE);

	// NOTE: P_estimate = P_estimate_temp_2 * P_prediction
	dspm_mult_f32_aes3(P_estimate_temp_2, P_prediction, P_estimate, P_SIZE, P_SIZE);
}


